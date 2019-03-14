#include "md.h"
#include "ui_md.h"
#include "dialogs/atomdialog.h"
#include "dialogs/plandialog.h"
#include "base/geometry.hpp"
#include "dialogs/pdfdialog.h"
#include "dialogs/smearingdialog.h"
#include "hist/histdatamd.hpp"

MD::MD(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::MD),
  _natom(0),
  _rmax(0),
  _smearing(100),
  _plot(this),
  _plotCommand(":plot "),
  _currentLeft(0),
  _currentRight(0)
{
  ui->setupUi(this);
  connect(&_plot, SIGNAL(newTab(PlotWindow::PlotPosition,int,QString)),this,SLOT(selectCombo(PlotWindow::PlotPosition,int,QString)));
  connect(&_plot, SIGNAL(removedTab(PlotWindow::PlotPosition,int)),this,SLOT(removeCombo(PlotWindow::PlotPosition,int)));
  QPlot* plot = nullptr;
  _plot.getPlot(PlotWindow::Right,-1,&plot);
  _plot.getPlot(PlotWindow::Left,-1,&plot);
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
  if (_plot.isHidden()) _plot.show();
  bool something = (view->getCanvas() != nullptr && view->getCanvas()->histdata() != nullptr);
  ui->gyration->setEnabled(something);
  bool isMD=false;
  bool isPIMD=false;
  if (something)
    {
      auto canvas = view->getCanvas();
      const HistData* hist = canvas->histdata();
      hist->waitTime(1);
      _natom = hist->natom();
      _rmax = geometry::getWignerSeitzRadius(hist->getRprimd(0));
      isPIMD=view->getCanvas()->histdata()->nimage()>1;
      isMD=(dynamic_cast<const HistDataMD*>(hist)!=nullptr);
    }
  ui->gyration->setEnabled(isPIMD);
  ui->thermodynamics->setEnabled(isMD);
  ui->vacf->setEnabled(isMD);
  ui->pdos->setEnabled(isMD);
}

void MD::setCombo()
{
  _autoUpdate = true;
  PlotWindow::PlotPosition pos;
  pos = (ui->left->isChecked()) ? PlotWindow::Left : PlotWindow::Right;
  auto names = _plot.getPlots(pos);
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
  _plot.getPlot(pos,index,&plot);
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
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" angle"));
}

void MD::on_plot_activated(int index)
{
  _plotCommand = ui->plot->itemData(index).toString();
}

void MD::on_lattice_clicked()
{
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" acell"));
}

void MD::on_gyration_clicked()
{
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" gyration"));
}

void MD::on_distance_clicked()
{
  if (_plot.isHidden()) _plot.show();
  AtomDialog dialog(2,_natom,this);
  if (dialog.exec()==QDialog::Accepted)
    {
      auto atoms = dialog.atoms();
      QString data;
      for (int i = 0; i < 2; ++i)
        {
          data += " "+QString::number(atoms[i]);
        }
      emit(sendCommand(_plotCommand+" distance"+data));
    }
}

void MD::on_angles_clicked()
{
  if (_plot.isHidden()) _plot.show();
  AtomDialog dialog(3,_natom,this);
  if (dialog.exec()==QDialog::Accepted)
    {
      auto atoms = dialog.atoms();
      QString data;
      for (int i = 0; i < 3; ++i)
        {
          data += " "+QString::number(atoms[i]);
        }
      emit(sendCommand(_plotCommand+" angle"+data));
    }
}

void MD::on_volume_clicked()
{
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" V"));
}

void MD::on_temperature_clicked()
{
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" T"));
}

void MD::on_pressure_clicked()
{
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" P"));
}

void MD::on_entropy_clicked()
{
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" entropy"));
}

void MD::on_energy_clicked()
{
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" etotal"));
}

void MD::on_kinetic_clicked()
{
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" ekin"));
}

void MD::on_stress_clicked()
{
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" stress"));
}

void MD::on_thermo_clicked()
{
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" thermo"));
}

void MD::on_positions_clicked()
{
  PlanDialog dialog(this);
  if (dialog.exec()!=QDialog::Accepted) return;
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" positions "+dialog.plan()+" "+dialog.coordinates()));
}

void MD::on_pdf_clicked()
{
  PdfDialog dialog(_rmax,_rmax/1000,this);
  if (dialog.exec()!=QDialog::Accepted) return;
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" g(r) "+QString::number(dialog.Rmax())+ " "+ QString::number(dialog.dR())));
}

void MD::on_msd_clicked()
{
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" msd"));
}

void MD::on_vacf_clicked()
{
  if (_plot.isHidden()) _plot.show();
  emit(sendCommand(_plotCommand+" vacf"));
}

void MD::on_pdos_clicked()
{
  if (_plot.isHidden()) _plot.show();
  SmearingDialog dialog(this);
  if (dialog.exec()!=QDialog::Accepted) return;
  emit(sendCommand(_plotCommand+" pdos"));
}
