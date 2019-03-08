#include "supercelldialog.h"
#include "ui_supercelldialog.h"
#include "base/geometry.hpp"
#include <QPushButton>

SupercellDialog::SupercellDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SupercellDialog),
  _rejected(false)
{
  ui->setupUi(this);
  this->setWindowTitle(tr("Set supercel matrix"));
  connect(ui->xx,SIGNAL(textChanged(QString)),this,SLOT(validate(QString)));
  connect(ui->xy,SIGNAL(textChanged(QString)),this,SLOT(validate(QString)));
  connect(ui->xz,SIGNAL(textChanged(QString)),this,SLOT(validate(QString)));
  connect(ui->yx,SIGNAL(textChanged(QString)),this,SLOT(validate(QString)));
  connect(ui->yy,SIGNAL(textChanged(QString)),this,SLOT(validate(QString)));
  connect(ui->yz,SIGNAL(textChanged(QString)),this,SLOT(validate(QString)));
  connect(ui->zx,SIGNAL(textChanged(QString)),this,SLOT(validate(QString)));
  connect(ui->zy,SIGNAL(textChanged(QString)),this,SLOT(validate(QString)));
  connect(ui->zz,SIGNAL(textChanged(QString)),this,SLOT(validate(QString)));
}

SupercellDialog::~SupercellDialog()
{
  delete ui;
}

QVector<int> SupercellDialog::matrix() const
{
  QVector<int> mat;
  if (!_rejected)
    {
      mat.resize(9);
      mat[0] = ui->xx->text().toInt();
      mat[1] = ui->xy->text().toInt();
      mat[2] = ui->xz->text().toInt();
      mat[3] = ui->yx->text().toInt();
      mat[4] = ui->yy->text().toInt();
      mat[5] = ui->yz->text().toInt();
      mat[6] = ui->zx->text().toInt();
      mat[7] = ui->zy->text().toInt();
      mat[8] = ui->zz->text().toInt();
    }
  return mat;
}

void SupercellDialog::on_buttonBox_rejected()
{
  _rejected = true;
}

void SupercellDialog::validate(QString dummy)
{
  (void) dummy;
  geometry::mat3d mat;
  mat[0] = ui->xx->text().toInt();
  mat[1] = ui->xy->text().toInt();
  mat[2] = ui->xz->text().toInt();
  mat[3] = ui->yx->text().toInt();
  mat[4] = ui->yy->text().toInt();
  mat[5] = ui->yz->text().toInt();
  mat[6] = ui->zx->text().toInt();
  mat[7] = ui->zy->text().toInt();
  mat[8] = ui->zz->text().toInt();
  bool valid = (std::abs(geometry::det(mat))>1e-6);
  QPushButton* ok = ui->buttonBox->button(QDialogButtonBox::Ok);
  ok->setEnabled(valid);
}
