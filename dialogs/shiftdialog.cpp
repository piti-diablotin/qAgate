#include "shiftdialog.h"
#include "ui_shiftdialog.h"

ShiftDialog::ShiftDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ShiftDialog),
  _rejected(false)
{
  ui->setupUi(this);
}

ShiftDialog::~ShiftDialog()
{
  delete ui;
}

void ShiftDialog::result(double &x, double &y, double &z) const
{
  x=ui->x->value();
  y=ui->y->value();
  z=ui->z->value();
}

void ShiftDialog::on_buttonBox_rejected()
{
   _rejected = 0;
   ui->x->setValue(0);
   ui->y->setValue(0);
   ui->z->setValue(0);
}
