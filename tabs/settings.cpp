#include "settings.h"
#include "ui_settings.h"
#include "canvas/canvaspos.hpp"
#include "dialogs/mendeleev.h"

Settings::Settings(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::Settings),
  _bondRadiusUnit(UnitConverter::bohr),
  _mendeleev(this)
{
  ui->setupUi(this);

  ui->antialiasing->setDefaultAction(ui->actionAA);
  ui->filling->setDefaultAction(ui->actionFilling);
  ui->light->setDefaultAction(ui->actionLight);
  ui->perspective->setDefaultAction(ui->actionPerspective);
  ui->mendeleev->setDefaultAction(ui->actionMendeleev);

  ui->actionAA->setChecked(true);
  ui->actionFilling->setChecked(true);
  ui->actionLight->setChecked(true);
  ui->actionPerspective->setChecked(false);

  this->addAction(ui->actionAA);
  this->addAction(ui->actionFilling);
  this->addAction(ui->actionLight);
  this->addAction(ui->actionPerspective);
  this->addAction(ui->actionMendeleev);

  connect(&_mendeleev,SIGNAL(accepted()),this,SLOT(updateMendeleev()));
}

Settings::~Settings()
{
  delete ui;
}

void Settings::updateStatus(View* view)
{
// Images
  ImageSaver::ImageType format;
  int quality;
  View::ImageSuffix suffix;
  view->imageSaverInfo(format,quality,suffix);
  ui->format->setCurrentIndex((int)format);
  ui->suffix->setCurrentIndex((int)suffix);
  ui->quality->setValue(quality);
// OpenGL
  ui->ndiv->setValue(view->canvas()->ndiv());
//display
  ui->width->setValue(view->width());
  ui->height->setValue(view->height());
  try
  {
    ui->size->setValue(view->option<int>("fontSize"));
  }
  catch(...){;}
// Atoms
  double rad, factor;
  CanvasPos* canvas;
  if ((canvas = dynamic_cast<CanvasPos*>(view->canvas())))
    {
      canvas->getBondInfo(rad,factor);
      ui->bond->setValue(factor);
      UnitConverter tmp = _bondRadiusUnit;
      tmp.rebase((UnitConverter::bohr));
      ui->bondRadius->setValue(rad*_bondRadiusUnit);
      ui->bond->setValue(factor);
    }
}

void Settings::updateDisplaySize(int width, int height)
{
  _autoUpdate = true;
  if (ui->width->value() != width) ui->width->setValue(width);
  if (ui->height->value() != height) ui->height->setValue(height);
  _autoUpdate = false;
}

void Settings::on_actionPerspective_triggered()
{
  emit(switchPerspective());
}

void Settings::on_actionLight_triggered()
{
   emit(switchLight());
}

void Settings::on_actionFilling_triggered()
{
  emit(switchFilling());
}

void Settings::on_actionAA_triggered()
{
   emit(switchAA());
}

void Settings::on_ndiv_valueChanged(int arg1)
{
  if (!_autoUpdate) emit(sendCommand(":div "+QString::number(arg1)));
}

void Settings::on_format_currentIndexChanged(const QString &arg1)
{
  if (!_autoUpdate) emit(sendCommand(":image_format "+arg1));
}

void Settings::on_suffix_currentIndexChanged(const QString &arg1)
{
  if (!_autoUpdate) emit(sendCommand(":image_suffix "+arg1));
}

void Settings::on_quality_valueChanged(const QString &arg1)
{
  if (!_autoUpdate) emit(sendCommand(":image_quality "+arg1));
}

void Settings::on_size_valueChanged(const QString &arg1)
{
   if (!_autoUpdate) emit(sendCommand(":fontsize "+arg1));
}

void Settings::on_ndiv_valueChanged(const QString &arg1)
{
   if (!_autoUpdate) emit(sendCommand(":division "+arg1));
}

void Settings::on_background_clicked()
{
  QColor color(0,0,0);
  color = QColorDialog::getColor(color,this,tr("Background color"),QColorDialog::DontUseNativeDialog);
  if (!color.isValid()) return;
  QString command = ":background " + QString::number(color.red())
      + " " + QString::number(color.green())
      + " " + QString::number(color.blue());
  emit(sendCommand(command));
}

void Settings::on_foreground_clicked()
{
  QColor color(0,0,0);
  color = QColorDialog::getColor(color,this,tr("Foreground color"),QColorDialog::DontUseNativeDialog);
  if (!color.isValid()) return;
  QString command = ":foreground " + QString::number(color.red())
      + " " + QString::number(color.green())
      + " " + QString::number(color.blue());
  emit(sendCommand(command));
}
/*
void Settings::on_width_valueChanged(const QString &arg1)
{
   if (!_autoUpdate) emit(sendCommand(":width "+arg1));
}

void Settings::on_height_valueChanged(const QString &arg1)
{
   if (!_autoUpdate) emit(sendCommand(":height "+arg1));
}
i*/
void Settings::on_width_editingFinished()
{
  if (!_autoUpdate) emit(sendCommand(":width "+QString::number(ui->width->value())));
}

void Settings::on_height_editingFinished()
{
  if (!_autoUpdate) emit(sendCommand(":height "+QString::number(ui->height->value())));
}

void Settings::on_bond_valueChanged(double arg1)
{
   if (!_autoUpdate) emit(sendCommand(":bond "+QString::number(arg1)));
}

void Settings::on_bondRadius_valueChanged(double arg1)
{
  if (!_autoUpdate)
    {
      double radBohr = arg1/_bondRadiusUnit;
      emit(sendCommand(":rad bond "+QString::number(radBohr)));
    }
}

void Settings::on_comboBox_newUnit(UnitConverter::Unit unit)
{
    UnitConverter newUnit(unit);
    newUnit.rebase(UnitConverter::bohr);
    double rad = ui->bondRadius->value()/_bondRadiusUnit*newUnit;
    _bondRadiusUnit = newUnit;
    ui->bondRadius->setValue(rad);
}

void Settings::on_actionMendeleev_triggered()
{
  _mendeleev.build();
  _mendeleev.show();
}

void Settings::updateMendeleev()
{
  QStringList modifs = _mendeleev.modifications();
  for ( auto it = modifs.begin(); it != modifs.end(); ++it)
    {
      emit(sendCommand(*it,false));
    }
  emit(sendCommand("r",false));
}

void Settings::on_up_clicked()
{
  QColor color(255,127,0);
  color = QColorDialog::getColor(color,this,tr("Spin up color"),QColorDialog::DontUseNativeDialog);
  if (!color.isValid()) return;
  QString command = ":color up " + QString::number(color.red())
      + " " + QString::number(color.green())
      + " " + QString::number(color.blue());
  emit(sendCommand(command));
}

void Settings::on_down_clicked()
{
  QColor color(0,255,127);
  color = QColorDialog::getColor(color,this,tr("Spin down color"),QColorDialog::DontUseNativeDialog);
  if (!color.isValid()) return;
  QString command = ":color down " + QString::number(color.red())
      + " " + QString::number(color.green())
      + " " + QString::number(color.blue());
  emit(sendCommand(command));
}
