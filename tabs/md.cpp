#include "md.h"
#include "ui_md.h"
#include "dialogs/atomdialog.h"
#include "dialogs/plandialog.h"
#include "base/geometry.hpp"
#include "dialogs/pdfdialog.h"
#include "dialogs/smearingdialog.h"
#include "hist/histdatamd.hpp"
#include "dialogs/straindialog.h"
#include "dialogs/stressdialog.h"
#include "dialogs/polarizationdialog.h"
#include "canvas/canvaslocal.hpp"

MD::MD(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::MD),
  _natom(0),
  _rmax(0),
  _smearing(100),
  _currentLeft(0),
  _currentRight(0),
  _xyMode(false),
  _xyPlot()
{
  ui->setupUi(this);
  bool buildPlotter = false;
  if (_plot == nullptr )
  {
    buildPlotter = true;
    _plot = new PlotWindow(this);
  }
  connect(_plot, SIGNAL(newTab(PlotWindow::PlotPosition,int,QString)),this,SLOT(selectCombo(PlotWindow::PlotPosition,int,QString)));
  connect(_plot, SIGNAL(removedTab(PlotWindow::PlotPosition,int)),this,SLOT(removeCombo(PlotWindow::PlotPosition,int)));
  if (buildPlotter)
  {
    QPlot* plot = nullptr;
    _plot->getPlot(PlotWindow::Right,-1,&plot);
    _plot->getPlot(PlotWindow::Left,-1,&plot);
  }
  ui->plot->setItemData(0,":plot ");
  ui->plot->setItemData(1,":print ");
  ui->plot->setItemData(2,":data ");
}

MD::~MD()
{
  delete ui;
}

void MD::updateStatus(View *view)
{
  this->setGraph();
  //if (_plot->isHidden()) _plot->show();
  bool something = (view->getCanvas() != nullptr && view->getCanvas()->histdata() != nullptr);
  ui->geometry->setEnabled(something);
  ui->statistics->setEnabled(something);
  ui->thermodynamics->setEnabled(something);
  ui->interpolation->setEnabled(something);
  ui->others->setEnabled(something);
  bool isMD=false;
  bool isPIMD=false;
  bool hasEtotal=false;
  bool hasStress=false;
  if (something)
    {
      auto canvas = view->getCanvas();
      const HistData* hist = canvas->histdata();
      hist->waitTime(1);
      _natom = hist->natom();
      _rmax = geometry::getWignerSeitzRadius(hist->getRprimd(0));
      isPIMD=view->getCanvas()->histdata()->nimage()>1;
      isMD=(dynamic_cast<const HistDataMD*>(hist)!=nullptr);
      hasEtotal=hist->hasEtotal();
      hasStress=hist->hasStress();
      ui->msd->setEnabled(hist->ntime()>1);
      ui->interpolation->setEnabled(hist->ntime()>1);
      ui->rotations->setEnabled(dynamic_cast<const CanvasLocal*>(canvas)!=nullptr);
    }
  ui->gyration->setEnabled(isPIMD);
  //ui->thermodynamics->setEnabled(isMD||hasEtotal);
  ui->energy->setEnabled(hasEtotal);
  ui->stress->setEnabled(hasStress);
  ui->temperature->setEnabled(isMD);
  ui->pressure->setEnabled(isMD);
  ui->kinetic->setEnabled(isMD);
  ui->entropy->setEnabled(isMD);
  ui->thermo->setEnabled(isMD);
  ui->vacf->setEnabled(isMD);
  ui->pdos->setEnabled(isMD);
}

void MD::plotFunction(const QString& function)
{
  if (_xyMode)
  {
    if (_xyPlot[0].isEmpty())
    {
      _xyPlot[0] = function;
      emit(statusMessage(tr("Select y function"),-1));
    }
    else
    {
      _xyPlot[1] = function;
      QString fullFunction="xy x=\"%0\" y=\"%1\"";
      fullFunction = fullFunction.arg(_xyPlot[0],_xyPlot[1]);
      ui->xy->toggle();
      this->plotFunction(fullFunction);
    }
  }
  else
  {
    if (_plot->isHidden()) _plot->show();
    QString hold = ui->hold->isChecked()? "true" : "false";
    emit(sendCommand(_plotCommand+function+QString(" hold=%0").arg(hold)));
  }
}

void MD::setCombo()
{
  _autoUpdate = true;
  PlotWindow::PlotPosition pos;
  pos = (ui->left->isChecked()) ? PlotWindow::Left : PlotWindow::Right;
  auto names = _plot->getPlots(pos);
  ui->graphCombo->clear();
  ui->graphCombo->insertItem(0,tr("New Graph"),-1);
  for (int i=0; i<names.size(); ++i)
    {
      ui->graphCombo->insertItem(i+1,names[i],i);
    }
  _autoUpdate = false;
}

void MD::selectCombo(PlotWindow::PlotPosition pos, int index, QString name)
{
  //bool updateCombo=false;
  if (pos == PlotWindow::Left)
    {
      //updateCombo = ui->left->isChecked();
      ui->left->setChecked(true);
      _currentLeft = index+1;
    }
  else
    {
      //inupdateCombo = ui->right->isChecked();
      ui->right->setChecked(true);
      _currentRight = index+1;
    }
  if (ui->graphCombo->count() <= index+1) this->setCombo();
  ui->graphCombo->setCurrentIndex(index+1);
  (void) name;
}

void MD::removeCombo(PlotWindow::PlotPosition pos, int index)
{
  PlotWindow::PlotPosition currentPos = (ui->left->isChecked()) ? PlotWindow::Left : PlotWindow::Right;
  if (pos!=currentPos) return;
  ui->graphCombo->removeItem(index+1);
  if (currentPos == PlotWindow::Left)
    _currentLeft = index+1;
  else
    _currentRight = index+1;
}

void MD::setGraph()
{
  int index = ui->graphCombo->currentData().toInt();
  PlotWindow::PlotPosition pos = (ui->left->isChecked()) ? PlotWindow::Left : PlotWindow::Right;
  QPlot* plot = nullptr;
  _plot->getPlot(pos,index,&plot);
  if (index!=-1) emit(plotChanged(plot));
}

void MD::on_left_toggled(bool checked)
{
  if (!checked) return;
  this->setCombo();
  _autoUpdate = true;
  ui->graphCombo->setCurrentIndex(_currentLeft);
  _autoUpdate = false;
  if (_currentLeft!=0) this->setGraph();
}

void MD::on_right_toggled(bool checked)
{
  if (!checked) return;
  this->setCombo();
  _autoUpdate = true;
  ui->graphCombo->setCurrentIndex(_currentRight);
  _autoUpdate = false;
  if (_currentRight!=0) this->setGraph();
}

void MD::on_graphCombo_currentIndexChanged(int index)
{
  (void) index;
  if (!_autoUpdate && index>=0) this->setGraph();
}

void MD::on_angle_clicked()
{
  this->plotFunction("angle");
}

void MD::on_plot_activated(int index)
{
  _plotCommand = ui->plot->itemData(index).toString()+" ";
}

void MD::on_lattice_clicked()
{
  this->plotFunction("acell");
}

void MD::on_gyration_clicked()
{
  this->plotFunction("gyration");
}

void MD::on_distance_clicked()
{
  AtomDialog dialog(2,_natom,this);
  if (dialog.exec()==QDialog::Accepted)
    {
      auto atoms = dialog.atoms();
      QString data;
      for (int i = 0; i < 2; ++i)
        {
          data += " "+QString::number(atoms[i]);
        }
      this->plotFunction("distance"+data);
    }
}

void MD::on_angles_clicked()
{
  AtomDialog dialog(3,_natom,this);
  if (dialog.exec()==QDialog::Accepted)
    {
      auto atoms = dialog.atoms();
      QString data;
      for (int i = 0; i < 3; ++i)
        {
          data += " "+QString::number(atoms[i]);
        }
      this->plotFunction("angle"+data);
    }
}

void MD::on_volume_clicked()
{
  this->plotFunction("V");
}

void MD::on_temperature_clicked()
{
  this->plotFunction("T");
}

void MD::on_pressure_clicked()
{
  this->plotFunction("P");
}

void MD::on_entropy_clicked()
{
  this->plotFunction("entropy");
}

void MD::on_energy_clicked()
{
  this->plotFunction("etotal");
}

void MD::on_kinetic_clicked()
{
  this->plotFunction("ekin");
}

void MD::on_stress_clicked()
{
  StressDialog dialog(this);
  if (dialog.exec()!=QDialog::Accepted) return;
  this->plotFunction(QString("stress only=%0").arg(dialog.only()));
}

void MD::on_thermo_clicked()
{
  this->plotFunction("thermo");
}

void MD::on_positions_clicked()
{
  PlanDialog dialog(this);
  if (dialog.exec()!=QDialog::Accepted) return;
  this->plotFunction("positions "+dialog.plan()+" "+dialog.coordinates());
}

void MD::on_pdf_clicked()
{
  PdfDialog dialog(_rmax,_rmax/1000,this);
  if (dialog.exec()!=QDialog::Accepted) return;
  this->plotFunction("g(r) "+QString::number(dialog.Rmax())+ " "+ QString::number(dialog.dR()));
}

void MD::on_msd_clicked()
{
  this->plotFunction("msd");
}

void MD::on_vacf_clicked()
{
  this->plotFunction("vacf");
}

void MD::on_pdos_clicked()
{
  SmearingDialog dialog(this);
  if (dialog.exec()!=QDialog::Accepted) return;
  QString data = "tsmear "+QString::number(dialog.smearing());
  this->plotFunction("pdos "+ data);
}

void MD::on_interpolate_clicked()
{
  emit(sendCommand(QString(":interpolate npoints=%0 amplitude=%1").arg(ui->npoints->text(),QString::number(ui->amplitude->value()))));
}

void MD::on_strain_clicked()
{
  StrainDialog dialog(this);
  dialog.setCurrentFolder(_currentFolder);
  if (dialog.exec()!=QDialog::Accepted) return;
  this->plotFunction(QString("strain reference=%0 time=%1 only=%2").arg(dialog.reference(),dialog.time(),dialog.only()));
}

void MD::on_polarization_clicked()
{
  PolarizationDialog dialog(this);
  dialog.setCurrentFolder(_currentFolder);
  if (dialog.exec()!=QDialog::Accepted) return;
  this->plotFunction(QString(" polarization ddb=%0").arg(dialog.ddb()));
}

void MD::on_rotations_clicked()
{
  this->plotFunction("rotations");
}

void MD::on_xy_toggled(bool checked)
{
  _xyMode = checked;
  if ( !checked )
  {
    _xyPlot[0].clear();
    _xyPlot[1].clear();
  }
  else
    emit(statusMessage(tr("Select x function"),-1));
}
