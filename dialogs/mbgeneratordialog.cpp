#include "mbgeneratordialog.h"
#include "ui_mbgeneratordialog.h"
#include <QIntValidator>
#include <QMessageBox>

MBGeneratorDialog::MBGeneratorDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::MBGeneratorDialog)
{
  ui->setupUi(this);
  ui->seed->setValidator(new QIntValidator(this));
  ui->seedType->setItemData(0,"random");
  ui->seedType->setItemData(1,"user");
  ui->seedType->setItemData(2,"time");
  ui->seedType->setItemData(3,"none");
  ui->distribution->setItemData(0,"normal");
  ui->distribution->setItemData(1,"uniform");
  ui->statistics->setItemData(0,"classical");
  ui->statistics->setItemData(1,"quantum");
  this->setModal(false);
}

MBGeneratorDialog::~MBGeneratorDialog()
{
  delete ui;
}

QString MBGeneratorDialog::seedType() const
{
  return ui->seedType->currentData().toString();
}

int MBGeneratorDialog::seed() const
{
  return ui->seed->text().toInt();
}

QString MBGeneratorDialog::distribution() const
{
  return ui->distribution->currentData().toString();
}

QString MBGeneratorDialog::statistics() const
{
  return ui->statistics->currentData().toString();
}

void MBGeneratorDialog::on_seedType_currentIndexChanged(int index)
{
  bool enable = (index==1);
  ui->seed->setEnabled(enable);
  ui->seedLabel->setEnabled(enable);
}

void MBGeneratorDialog::on_seed_editingFinished()
{
    if (ui->seed->text().isEmpty())
    {
      QMessageBox::information(this,tr("Missing data"),tr("You need to give the seed"));
      return;
    }
}
