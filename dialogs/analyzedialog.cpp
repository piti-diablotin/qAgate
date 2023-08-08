#include "analyzedialog.h"
#include "ui_analyzedialog.h"
#include <QFileDialog>
#include <QRegularExpression>

AnalyzeDialog::AnalyzeDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AnalyzeDialog)
{
  ui->setupUi(this);
  QString label = ui->absolute->text();
  ui->absolute->setText(label.replace("Gamma",QChar(0x93,0x03)));
}

AnalyzeDialog::~AnalyzeDialog()
{
  delete ui;
}

QString AnalyzeDialog::normalization()
{
   if (ui->no->isChecked())
   {
     return QString();
   }
   else if (ui->normalized->isChecked())
   {
     return QString("normalized");
   }
   else
     return QString("fullnormalized");
}

QString AnalyzeDialog::file()
{
  return ui->file->text();
}

bool AnalyzeDialog::absolute()
{
  return ui->absolute->isChecked();
}

void AnalyzeDialog::on_browse_clicked()
{
  auto fileName = QFileDialog::getOpenFileName(this,"Open File",_currentFolder,INPUT_FILES_FILTER,nullptr,QFileDialog::DontUseNativeDialog);
  if ( !fileName.isEmpty() )
  {
    ui->file->setText(fileName);
    int pos = fileName.lastIndexOf(QRegularExpression("[/\\\\]"));
    _currentFolder = fileName.left(pos+1);
  }
}

QString AnalyzeDialog::currentFolder() const
{
  return _currentFolder;
}

void AnalyzeDialog::setCurrentFolder(const QString &currentFolder)
{
  _currentFolder = currentFolder;
}
