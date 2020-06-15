#include "visuals.h"
#include "ui_visuals.h"
#include "base/phys.hpp"
#include "canvas/canvaspos.hpp"
#include "canvas/canvaslocal.hpp"
#include "base/mendeleev.hpp"

Visuals::Visuals(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::Visuals),
  _znuclOrder(),
  _octaColor()
{
  ui->setupUi(this);

  ui->px->setDefaultAction(ui->action_px);
  ui->py->setDefaultAction(ui->action_py);
  ui->pz->setDefaultAction(ui->action_pz);
  ui->mx->setDefaultAction(ui->action_mx);
  ui->my->setDefaultAction(ui->action_my);
  ui->mz->setDefaultAction(ui->action_mz);
  ui->x->setDefaultAction(ui->actionX);
  ui->y->setDefaultAction(ui->actionY);
  ui->z->setDefaultAction(ui->actionZ);
  ui->time->setDefaultAction(ui->actionTime);
  ui->angles->setDefaultAction(ui->actionAngles);

  ui->actionAngles->setChecked(true);
  ui->actionTime->setChecked(true);

  this->addAction(ui->action_px);
  this->addAction(ui->action_py);
  this->addAction(ui->action_pz);
  this->addAction(ui->action_mx);
  this->addAction(ui->action_my);
  this->addAction(ui->action_mz);
  this->addAction(ui->actionX);
  this->addAction(ui->actionY);
  this->addAction(ui->actionZ);
  this->addAction(ui->actionAngles);
  this->addAction(ui->actionTime);

  QPalette pal = ui->octaColor->palette();
  _octaColor.setRgbF(0.5,0.5,0.5,0.8);
  pal.setColor(QPalette::Button,_octaColor);
  ui->octaColor->setPalette(pal);
  ui->spin->setItemData(0,"none",Qt::UserRole);
  for (int i = 1 ; i < 8 ; ++i)
    ui->spin->setItemData(i,ui->spin->itemData(i,Qt::DisplayRole),Qt::UserRole);
}

Visuals::~Visuals()
{
  delete ui;
}

void Visuals::updateStatus(View *view)
{
 (void) view;
  try
  {
    bool axis = view->option<bool>("axis");
    bool axisCart = view->option<bool>("axisCart");
    ui->axis->setChecked(axis&&axisCart);
    ui->axisABC->setChecked(axis&&(!axisCart));
  }
  catch(...){;}
  unsigned display = view->getDisplay();
  ui->atom->setChecked(display & CanvasPos::DISP_ATOM);
  ui->bond->setChecked(display & CanvasPos::DISP_BOND);
  ui->border->setChecked(display & CanvasPos::DISP_BORDER);
  ui->incircle->setChecked(display & CanvasPos::DISP_INCIRCLE);
  ui->cell->setChecked(display & CanvasPos::DISP_CELL);
  ui->force->setChecked(display & CanvasPos::DISP_FORCE);
  if (display & CanvasPos::DISP_ID)
    ui->id->setChecked(true);
  else if (display & CanvasPos::DISP_ZNUCL)
    ui->znucl->setChecked(true);
  else if (display & CanvasPos::DISP_NAME)
    ui->name->setChecked(true);
  // Octahedra
  const CanvasPos* canvas = dynamic_cast<const CanvasPos*>(view->getCanvas());
  if (canvas!=nullptr)
  {
    bool x, y ,z, relative= false, centered=true;
    canvas->getSpinDirection(x,y,z,relative,centered);
    if (x&&y&&z)
      ui->spin->setCurrentIndex(7);
    else if (!x&&y&&z)
      ui->spin->setCurrentIndex(6);
    else if (x&&!y&&z)
      ui->spin->setCurrentIndex(5);
    else if (x&&y&&!z)
      ui->spin->setCurrentIndex(4);
    else if (!x&&!y&&z)
      ui->spin->setCurrentIndex(3);
    else if (!x&&y&&!z)
      ui->spin->setCurrentIndex(2);
    else if (x&&!y&&!z)
      ui->spin->setCurrentIndex(1);
    else if (!x&&!y&&!z)
      ui->spin->setCurrentIndex(0);
    ui->spinRelative->setChecked(relative);
    ui->spinCentered->setChecked(centered);

    ui->forceFactor->setValue(static_cast<int>(canvas->getForceFactor()));

    const HistData* hist = canvas->histdata();
    if ((hist)!=nullptr)
    {
      ui->octaAdd->clear();
      ui->octaDel->clear();
      ui->octaAdd->addItem(QIcon(":/visuals/icons/plus-circle.svg"),"");
      ui->octaDel->addItem(QIcon(":/visuals/icons/minus-circle.svg"),"");
      bool draw;
      auto octa = canvas->getOctahedra(draw);
      ui->atoms->setChecked(draw);
      auto znucl = hist->znucl();
      _znuclOrder = QVector<int>::fromStdVector(znucl);
      for (unsigned i = 0; i < znucl.size(); ++i)
      {
        auto found = std::find(octa.begin(),octa.end(),znucl[i]);
        QVariant data(znucl[i]);
        if (found == octa.end())
        {
          int newPos = 1;
          for (newPos=1 ; newPos<ui->octaAdd->count(); ++newPos)
            if (ui->octaAdd->itemData(newPos)>data) break;
          ui->octaAdd->insertItem(newPos,QString(MendeTable.name[znucl[i]]),data);
        }
        else
        {
          int newPos = 1;
          for (newPos=1 ; newPos<ui->octaDel->count(); ++newPos)
            if (ui->octaDel->itemData(newPos)>data) break;
          ui->octaDel->insertItem(newPos,QString(MendeTable.name[znucl[i]]),data);
        }
      }
    }
    QPalette pal = ui->octaColor->palette();
    auto octaColor = canvas->getOctahedraColor();
    _octaColor.setRgbF(octaColor[0],octaColor[1],octaColor[2],octaColor[3]);
    pal.setColor(QPalette::Button,_octaColor);
    ui->octaColor->setPalette(pal);
    }
  const CanvasLocal* canvasLoc = dynamic_cast<const CanvasLocal*>(view->getCanvas());
  if (canvasLoc == nullptr && canvas != nullptr)
    {
      ui->octaColor->setEnabled(true);
    }
  else
    {
      ui->octaColor->setEnabled(false);
    }
}


void Visuals::on_axis_clicked()
{
  bool checked = ui->axis->isChecked();
  if (checked) ui->axisABC->setChecked(false);
  emit(sendCommand(checked ? ":axis cartesian" : ":axis"));
}

void Visuals::on_actionTime_triggered()
{
  emit(switchTimeInfo());
}

void Visuals::on_actionAngles_triggered()
{
  emit(switchAngles()) ;
}

void Visuals::on_action_px_triggered()
{
  emit(translatePX());
}

void Visuals::on_action_py_triggered()
{
  emit(translatePY());
}

void Visuals::on_action_pz_triggered()
{
  emit(translatePZ());
}

void Visuals::on_action_mz_triggered()
{
  emit(translateMZ());
}

void Visuals::on_action_my_triggered()
{
  emit(translateMY());
}

void Visuals::on_action_mx_triggered()
{
  emit(translateMX());
}

void Visuals::on_actionX_triggered()
{
  ui->psi->setValue(0);
  ui->theta->setValue(0);
  ui->phi->setValue(0);
  emit(alongX());
}

void Visuals::on_actionY_triggered()
{
  ui->psi->setValue(0);
  ui->theta->setValue(0);
  ui->phi->setValue(90);
  emit(alongY());
}

void Visuals::on_actionZ_triggered()
{
  ui->psi->setValue(-90);
  ui->theta->setValue(0);
  ui->phi->setValue(90);
  emit(alongZ());
}

void Visuals::updateAngles(View *view)
{
  auto psi = view->option<float>("campsi");
  auto theta = view->option<float>("camtheta");
  auto phi = view->option<float>("camphi");
  _autoUpdate = true;
  ui->psi->setValue((int)std::round(psi/phys::pi*180));
  ui->theta->setValue((int)std::round(theta/phys::pi*180));
  ui->phi->setValue((int)std::round(phi/phys::pi*180));
  _autoUpdate = false;
}

void Visuals::on_psi_valueChanged(int arg1)
{
  if (!_autoUpdate) emit(sendCommand(":psi "+QString::number(arg1)));
}

void Visuals::on_theta_valueChanged(int arg1)
{
  if (!_autoUpdate) emit(sendCommand(":theta "+QString::number(arg1)));
}

void Visuals::on_phi_valueChanged(int arg1)
{
  if (!_autoUpdate) emit(sendCommand(":phi "+QString::number(arg1)));
}

void Visuals::on_displayGroup_buttonClicked(QAbstractButton *button)
{
  emit(sendCommand(":show "+button->objectName()));
}

void Visuals::on_optionsGroup_buttonClicked(QAbstractButton *button)
{
  emit(sendCommand((button->isChecked()?":show ":":hide ")+button->objectName()));
}

void Visuals::on_spin_currentIndexChanged(int index)
{
  if (!_autoUpdate)
    {
      QString direction = ui->spin->itemData(index,Qt::UserRole).toString();
      (direction == "none") ? direction = "" : direction = " "+direction;
      emit(sendCommand(":spin"+direction));
    }
}

void Visuals::on_octaAdd_activated(int index)
{
  if (index==0) return;
  QString atoms = ui->atoms->isChecked() ? " 1" : " 0";
  QString command=":octa_z "+ui->octaAdd->itemText(index)+atoms;
  auto data = ui->octaAdd->itemData(index);
  int newPos = 1;
  for (newPos=1 ; newPos<ui->octaDel->count(); ++newPos)
    if (ui->octaDel->itemData(newPos)>data) break;
  ui->octaDel->insertItem(newPos,ui->octaAdd->itemText(index),data);
  ui->octaAdd->removeItem(index);
  ui->octaAdd->setCurrentIndex(0);
  emit(sendCommand(command));
}

void Visuals::on_octaDel_activated(int index)
{
  if (index==0) return;
  QString atoms = ui->atoms->isChecked() ? " 1" : " 0";
  QString command=":octa_z -"+ui->octaDel->itemText(index)+atoms;
  auto data = ui->octaDel->itemData(index);
  int newPos = 1;
  for (newPos=1 ; newPos<ui->octaAdd->count(); ++newPos)
    if (ui->octaAdd->itemData(newPos)>data) break;
  ui->octaAdd->insertItem(newPos,ui->octaDel->itemText(index),data);
  ui->octaDel->removeItem(index);
  ui->octaDel->setCurrentIndex(0);
  emit(sendCommand(command));
}

void Visuals::on_atoms_clicked(bool checked)
{
  QString atoms = checked ? " 1" : " 0";
  if (ui->octaDel->count()>1)
    {
      QString command=":octa_z "+ui->octaDel->itemText(1)+atoms;
      emit(sendCommand(command));
    }
}

void Visuals::on_octaColor_clicked()
{
  QColor newColor = QColorDialog::getColor(_octaColor,this,tr("Pick a base color"),QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);
  if (!newColor.isValid()) return;
  _octaColor = newColor;
  QPalette pal = ui->octaColor->palette();
  pal.setColor(QPalette::Button,_octaColor);
  ui->octaColor->setPalette(pal);
  QString color = QString::number(newColor.red())
      + " " + QString::number(newColor.green())
      + " " + QString::number(newColor.blue())
      + " " + QString::number(newColor.alpha());
  emit(sendCommand(":color octa "+color));
}

void Visuals::on_spinRelative_clicked(bool checked)
{
  QString value = checked ? "relative" : "absolute";
  emit(sendCommand(":spin_length "+value));
}

void Visuals::on_axisABC_clicked()
{
  bool checked = ui->axisABC->isChecked();
  if (checked) ui->axis->setChecked(false);
  emit(sendCommand(checked ? ":axis lattice" : ":axis"));
}

void Visuals::on_cell_clicked(bool checked)
{
  emit(sendCommand(checked?":show cell":":hide cell"));
}

void Visuals::on_forceFactor_valueChanged(int value)
{
  if (!_autoUpdate)
    emit(sendCommand(QString(":force_scaling %0").arg(value)));
}

void Visuals::on_force_clicked(bool checked)
{
  emit(sendCommand(checked?":show force":":hide force"));
}

void Visuals::on_spinCentered_clicked(bool checked)
{
  emit(sendCommand(QString(":spin_origin centered=%0").arg(checked?"1":"0")));
}
