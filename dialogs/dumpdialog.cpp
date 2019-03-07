#include "dumpdialog.h"
#include "ui_dumpdialog.h"
#include <QFileDialog>

DumpDialog::DumpDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DumpDialog),
  _lastDirectory()
{
  ui->setupUi(this);
  this->setModal(true);
  this->setWindowTitle(tr("Export options - qAgate"));
}

DumpDialog::~DumpDialog()
{
  delete ui;
}

QString DumpDialog::filename() const
{
  return ui->filename->text();
}

int DumpDialog::step() const
{
  return ui->step->value();
}

QString DumpDialog::directory() const
{
  return _lastDirectory;
}

void DumpDialog::setDirectory(QString dir)
{
  _lastDirectory = dir;
}

void DumpDialog::on_browse_clicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), _lastDirectory);
  if (!fileName.isEmpty())
    {
      ui->filename->setText(fileName);
      _lastDirectory = fileName.section("/",0,-2);
    }
}

void DumpDialog::on_buttonBox_rejected()
{
   ui->filename->clear();
   ui->step->setValue(1);
}
