#include "stressdialog.h"
#include "ui_stressdialog.h"

StressDialog::StressDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::StressDialog)
{
  ui->setupUi(this);
}

StressDialog::~StressDialog()
{
  delete ui;
}

QString StressDialog::only() const
{
  return QString::number(ui->only->currentIndex());
}
