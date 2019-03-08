#include "spgdialog.h"
#include "ui_spgdialog.h"
#include <QDoubleValidator>

SpgDialog::SpgDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SpgDialog),
  _validator(1e-8,1,8,this),
  _rejected(false)
{
  ui->setupUi(this);
  this->setWindowTitle(tr("Set spacegroup precision"));
  ui->precision->setValidator(&_validator);
}

SpgDialog::~SpgDialog()
{
  delete ui;
}

double SpgDialog::precision() const
{
  if (_rejected) return -1.0;
  UnitConverter unit(UnitConverter::bohr);
  unit = ui->unit->unit();
   return ui->precision->text().toDouble()/unit;
}

void SpgDialog::on_buttonBox_rejected()
{
   _rejected = true;
}
