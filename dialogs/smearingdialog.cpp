#include "smearingdialog.h"
#include "ui_smearingdialog.h"

SmearingDialog::SmearingDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SmearingDialog),
  _unit(UnitConverter::K)
{
  ui->setupUi(this);
  ui->unit->setCurrentText("K");
}

SmearingDialog::~SmearingDialog()
{
  delete ui;
}

double SmearingDialog::smearing() const
{
 return ui->tsmear->value()/_unit;
}

void SmearingDialog::on_unit_currentIndexChanged(int index)
{
  (void) index;
  UnitConverter newUnit(ui->unit->unit());
  newUnit.rebase(UnitConverter::K);
  double factor = 1/_unit*newUnit;
  double val = ui->tsmear->value();
  ui->tsmear->setRange(ui->tsmear->minimum()*factor,ui->tsmear->maximum()*factor);
  ui->tsmear->setValue(val*factor);
  _unit = newUnit;
}
