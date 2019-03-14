#include "movedialog.h"
#include "ui_movedialog.h"

MoveDialog::MoveDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::MoveDialog),
  _xred(),
  _rejected(false)
{
  ui->setupUi(this);
}

MoveDialog::~MoveDialog()
{
  delete ui;
}

void MoveDialog::configure(int natom, const double *xred)
{
  ui->iatom->setRange(1,natom);
  if (xred!=nullptr)
    {
      _xred.resize(3*natom);
      for (int i = 0; i < 3*natom; ++i)
        _xred[i] = xred[i];
      int currentAtom = ui->iatom->value();
      ui->x->setValue(_xred[3*currentAtom]);
      ui->y->setValue(_xred[3*currentAtom+1]);
      ui->z->setValue(_xred[3*currentAtom+2]);
    }
  else _xred.clear();
}

void MoveDialog::result(int &iatom, double &x, double &y, double &z) const
{
  if (_rejected)
    {
      iatom = 0;
      return;
    }
  iatom = ui->iatom->value();
  x = ui->x->value();
  y = ui->y->value();
  z = ui->z->value();
}

void MoveDialog::on_iatom_valueChanged(int arg1)
{
  if (_xred.isEmpty()) return;
  ui->x->setValue(_xred[3*arg1]);
  ui->y->setValue(_xred[3*arg1+1]);
  ui->z->setValue(_xred[3*arg1+2]);
}

void MoveDialog::on_buttonBox_rejected()
{
  _rejected = false;
}
