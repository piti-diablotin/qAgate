#include "straindialog.h"
#include "ui_straindialog.h"
#include <QFileDialog>
#include <QIntValidator>
#include "hist/histdata.hpp"
#include <QMessageBox>

StrainDialog::StrainDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::StrainDialog)
{
  ui->setupUi(this);
  ui->time->setValidator(new QIntValidator(0,0,this));
}

StrainDialog::~StrainDialog()
{
  delete ui;
}

void StrainDialog::on_browse_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),_currentFolder,INPUT_FILES_FILTER,nullptr,QFileDialog::DontUseNativeDialog);
  if ( !fileName.isEmpty() )
  {
    try {
      int pos = fileName.lastIndexOf(QRegExp("[/\\\\]"));
      _currentFolder = fileName.left(pos+1);
      HistData* tmp = HistData::getHist(fileName.toStdString());
      ui->time->setEnabled(tmp->ntime()>1);
      ui->time->setText("0");
      ui->time->setValidator(new QIntValidator(0,tmp->ntime()-1,this));
      delete tmp;
      ui->reference->setText(fileName);
    }
    catch( Exception &e ) {
      QMessageBox::critical(this,tr("Bad file"),tr("File is not recognized with a correct format"));
    }
  }
}

QString StrainDialog::currentFolder() const
{
  return _currentFolder;
}

void StrainDialog::setCurrentFolder(const QString &currentFolder)
{
  _currentFolder = currentFolder;
}

QString StrainDialog::reference() const
{
  return ui->reference->text();
}

QString StrainDialog::time() const
{
  return ui->time->text();
}

QString StrainDialog::only() const
{
  return QString::number(ui->only->currentIndex());
}
