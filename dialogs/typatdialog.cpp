#include "typatdialog.h"
#include "ui_typatdialog.h"
#include "base/mendeleev.hpp"

TypatDialog::TypatDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::TypatDialog),
  _rejected(false)
{
  ui->setupUi(this);
  this->setWindowTitle(tr("Change type"));
}

TypatDialog::~TypatDialog()
{
  delete ui;
}

void TypatDialog::configure(int natom, const QVector<int> &znucl)
{
  ui->iatom->setRange(1,natom);
  ui->typat->clear();
  for(int i=0; i<znucl.size(); ++i)
    {
      ui->typat->addItem(QString(Agate::mendeleev::name[znucl[i]]),i+1);
    }
}

void TypatDialog::result(int &iatom, int& znucl)
{
  if (_rejected) {
      iatom = 0;
      znucl = 0;
    }
  else
    {
      iatom = ui->iatom->value();
      znucl = ui->typat->currentData().toInt();
    }
}

void TypatDialog::on_buttonBox_rejected()
{
  _rejected=true;
}
