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
    ui->axis->setChecked(view->option<bool>("axis"));
  }
  catch(...){;}
  unsigned display = view->getDisplay();
  ui->atom->setChecked(display & CanvasPos::DISP_ATOM);
  ui->bond->setChecked(display & CanvasPos::DISP_BOND);
  ui->border->setChecked(display & CanvasPos::DISP_BORDER);
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
          ui->octaAdd->insertItem(newPos,QString(Agate::Mendeleev.name[znucl[i]]),data);
        }
        else
        {
          int newPos = 1;
          for (newPos=1 ; newPos<ui->octaDel->count(); ++newPos)
            if (ui->octaDel->itemData(newPos)>data) break;
          ui->octaDel->insertItem(newPos,QString(Agate::Mendeleev.name[znucl[i]]),data);
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
  emit(sendCommand(":axis"));
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
  emit(alongX());
}

void Visuals::on_actionY_triggered()
{
  emit(alongY());
}

void Visuals::on_actionZ_triggered()
{
  emit(alongZ());
}

void Visuals::updateAngles(View *view)
{
  auto psi = view->option<float>("campsi");
  auto theta = view->option<float>("camtheta");
  auto phi = view->option<float>("camphi");
  _autoUpdate = true;
  ui->psi->setValue((int)(psi/phys::pi*180));
  ui->theta->setValue((int)(theta/phys::pi*180));
  ui->phi->setValue((int)(phi/phys::pi*180));
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

void Visuals::on_spin_currentTextChanged(const QString &arg1)
{
  if (!_autoUpdate)
    {
      QString direction = arg1;
      (direction == "None") ? direction = "" : direction = " "+direction;
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
  QString color = QString::number(newColor.red())
      + " " + QString::number(newColor.green())
      + " " + QString::number(newColor.blue())
      + " " + QString::number(newColor.alpha());
  emit(sendCommand(":color octa "+color));
}