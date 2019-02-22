#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Settings),
  _autoUpdate(false)
{
  ui->setupUi(this);
}

Settings::~Settings()
{
  delete ui;
}

void Settings::updateStatus(const View &view)
{
// Images
  ui->format;
  ui->suffix;
  ui->quality;
// OpenGL
  ui->ndiv;
//display
  ui->size;
}

void Settings::updateDisplaySize(int width, int height)
{
  _autoUpdate = true;
  if (ui->width->value() != width) ui->width->setValue(width);
  if (ui->height->value() != height) ui->height->setValue(height);
  _autoUpdate = false;
}

void Settings::on_perspective_clicked()
{
  emit(switchPerspective());
}

void Settings::on_light_clicked()
{
   emit(switchLight());
}

void Settings::on_filling_clicked()
{
  emit(switchFilling());
}

void Settings::on_antialiasing_clicked()
{
   emit(switchAA());
}

void Settings::on_angles_clicked()
{
   emit(switchAngles()) ;
}

void Settings::on_ndiv_valueChanged(int arg1)
{
  if (!_autoUpdate) emit(sendCommand(":div "+QString::number(arg1)));

}

void Settings::on_format_currentIndexChanged(const QString &arg1)
{
  if (!_autoUpdate) emit(sendCommand(":image_format "+arg1));
}
