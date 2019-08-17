#include "densitymode.h"
#include "ui_densitymode.h"
#include "canvas/canvasdensity.hpp"

DensityMode::DensityMode(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::DensityMode)
{
  ui->setupUi(this);
  ui->function->setItemData(0,QString("linear"));
  ui->function->setItemData(1,QString("log"));
  ui->function->setItemData(2,QString("sqrt"));
}

DensityMode::~DensityMode()
{
  delete ui;
}

void DensityMode::updateStatus(View *view)
{
  const CanvasDensity* canvas = dynamic_cast<const CanvasDensity*>(view->getCanvas());
  this->setEnabled(canvas!=nullptr);
  if (canvas!=nullptr)
  {
    _autoUpdate = true;
    this->setDisabled(canvas->ntime()==0);
    switch(canvas->dispDen())
    {
      case AbiBin::UP:
        ui->up->setChecked(true);
        break;
      case AbiBin::DOWN:
        ui->down->setChecked(true);
        break;
      case AbiBin::SUM:
        ui->sum->setChecked(true);
        break;
      case AbiBin::DIFF:
        ui->diff->setChecked(true);
        break;
      case AbiBin::X:
        ui->x->setChecked(true);
        break;
      case AbiBin::Y:
        ui->y->setChecked(true);
        break;
      case AbiBin::Z:
        ui->z->setChecked(true);
        break;
    }
    switch (canvas->normal())
    {
       case AbiBin::A :
        ui->normal->setCurrentIndex(0);
        break;
       case AbiBin::B :
        ui->normal->setCurrentIndex(1);
        break;
       case AbiBin::C :
        ui->normal->setCurrentIndex(2);
        break;
    }
    switch (canvas->scaleFunction())
    {
       case CanvasDensity::linear :
        ui->function->setCurrentIndex(0);
        break;
       case CanvasDensity::log :
        ui->function->setCurrentIndex(1);
        break;
       case CanvasDensity::sqrt :
        ui->function->setCurrentIndex(2);
        break;
    }
    ui->factor->setText(QString::number(canvas->scaleValues()));
    ui->factor->setValidator(new QDoubleValidator(this));
    const float (*colors)[3] = canvas->colors();
    QPalette pal = ui->colorMin->palette();
    _minusColor.setRgbF(colors[0][0],colors[0][1],colors[0][2]);
    _zeroColor.setRgbF( colors[1][0],colors[1][1],colors[1][2]);
    _plusColor.setRgbF( colors[2][0],colors[2][1],colors[2][2]);
    pal.setColor(QPalette::Button,_minusColor);
    ui->colorMin->setPalette(pal);
    pal.setColor(QPalette::Button,_zeroColor);
    ui->colorZero->setPalette(pal);
    pal.setColor(QPalette::Button,_plusColor);
    ui->colorMax->setPalette(pal);
    _autoUpdate = false;
  }
}

void DensityMode::on_dispdenGroup_buttonClicked(QAbstractButton *button)
{
  if (!_autoUpdate) emit(sendCommand(":density "+button->objectName()));
}

void DensityMode::on_normal_currentIndexChanged(int index)
{
  if (!_autoUpdate) emit(sendCommand(":normal "+ui->normal->itemText(index)));
}

void DensityMode::on_function_currentIndexChanged(int index)
{
   if (!_autoUpdate) emit(sendCommand(":scale function "+ui->function->itemData(index).toString()));
}

void DensityMode::on_factor_editingFinished()
{
   if (!_autoUpdate) emit(sendCommand(":scale factor "+ui->factor->text())) ;
}

void DensityMode::on_colorMin_clicked()
{
  QColor newColor = QColorDialog::getColor(_minusColor,this,tr("Pick a color for minimum value (negative)"),QColorDialog::DontUseNativeDialog);
  if (!newColor.isValid()) return;
  _minusColor= newColor;
  QPalette pal = ui->colorMin->palette();
  pal.setColor(QPalette::Button,_minusColor);
  ui->colorMin->setPalette(pal);
  QString color = QString::number(newColor.red())
      + " " + QString::number(newColor.green())
      + " " + QString::number(newColor.blue());
  emit(sendCommand(":color down "+color));
}

void DensityMode::on_colorZero_clicked()
{
  QColor newColor = QColorDialog::getColor(_zeroColor,this,tr("Pick a color for zero value"),QColorDialog::DontUseNativeDialog);
  if (!newColor.isValid()) return;
  _zeroColor= newColor;
  QPalette pal = ui->colorZero->palette();
  pal.setColor(QPalette::Button,_zeroColor);
  ui->colorZero->setPalette(pal);
  QString color = QString::number(newColor.red())
      + " " + QString::number(newColor.green())
      + " " + QString::number(newColor.blue());
  emit(sendCommand(":color zero "+color));
}

void DensityMode::on_colorMax_clicked()
{
  QColor newColor = QColorDialog::getColor(_plusColor,this,tr("Pick a color for maximal value (positive)"),QColorDialog::DontUseNativeDialog);
  if (!newColor.isValid()) return;
  _plusColor= newColor;
  QPalette pal = ui->colorMax->palette();
  pal.setColor(QPalette::Button,_plusColor);
  ui->colorMax->setPalette(pal);
  QString color = QString::number(newColor.red())
      + " " + QString::number(newColor.green())
      + " " + QString::number(newColor.blue());
  emit(sendCommand(":color up "+color));
}
