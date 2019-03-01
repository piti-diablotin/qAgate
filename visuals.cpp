#include "visuals.h"
#include "ui_visuals.h"
#include "base/phys.hpp"

Visuals::Visuals(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::Visuals)
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