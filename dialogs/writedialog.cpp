#include "writedialog.h"
#include "ui_writedialog.h"
#include "base/unitconverter.hpp"
#include <QFileDialog>
#include <QDebug>

WriteDialog::WriteDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::WriteDialog)
{
  ui->setupUi(this);
}

WriteDialog::~WriteDialog()
{
  delete ui;
}

QString WriteDialog::filename() const
{
  return ui->filename->text();
}

QString WriteDialog::directory() const
{
  return _lastDirectory;
}

double WriteDialog::precision() const
{
  UnitConverter unit = UnitConverter::getFromString(ui->unit->currentText().toStdString());
  unit = UnitConverter::bohr;
  return ui->precision->value()*unit;
}

QString WriteDialog::option() const
{
  if (ui->current->isChecked())
    {
      return "";
    }
  else if (ui->standard->isChecked())
    {
      return "standard";
    }
  else if (ui->primitive->isChecked())
    {
      return "primitive";
    }
  else
    return "";
}

void WriteDialog::setDirectory(QString dir)
{
  _lastDirectory=dir;
}

void WriteDialog::on_browse_clicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), _lastDirectory);
  if (!fileName.isEmpty())
    {
      ui->filename->setText(fileName);
      _lastDirectory = fileName.section("/",0,-2);
    }
}

void WriteDialog::on_buttonBox_rejected()
{
   ui->filename->clear();
   ui->current->setChecked(true);
   ui->precision->setValue(0.001);
   ui->unit->setCurrentIndex(0);
}

void WriteDialog::on_current_toggled(bool checked)
{
   ui->precisionWidget->setDisabled(checked);
}

void WriteDialog::on_primitive_toggled(bool checked)
{
   ui->precisionWidget->setEnabled(checked);
}

void WriteDialog::on_standard_toggled(bool checked)
{
   ui->precisionWidget->setEnabled(checked);
}
