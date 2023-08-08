#include "polarizationdialog.h"
#include "ui_polarizationdialog.h"
#include "base/exception.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>

PolarizationDialog::PolarizationDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PolarizationDialog)
{
  ui->setupUi(this);
}

PolarizationDialog::~PolarizationDialog()
{
  delete ui;
}

void PolarizationDialog::on_browse_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),_currentFolder,DDB_FILES_FILTER,nullptr,QFileDialog::DontUseNativeDialog);
  if ( !fileName.isEmpty() )
  {
    try {
      int pos = fileName.lastIndexOf(QRegularExpression("[/\\\\]"));
      _currentFolder = fileName.left(pos+1);
      ui->ddb->setText(fileName);
    }
    catch( Exception &e ) {
      QMessageBox::critical(this,tr("Bad file"),tr("File is not recognized with a correct format"));
    }
  }

}

QString PolarizationDialog::currentFolder() const
{
  return _currentFolder;
}

void PolarizationDialog::setCurrentFolder(const QString &currentFolder)
{
  _currentFolder = currentFolder;
}

QString PolarizationDialog::ddb() const
{
  return ui->ddb->text();
}
