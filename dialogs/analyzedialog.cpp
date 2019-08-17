#include "analyzedialog.h"
#include "ui_analyzedialog.h"
#include <QFileDialog>

AnalyzeDialog::AnalyzeDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AnalyzeDialog)
{
  ui->setupUi(this);
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

void AnalyzeDialog::on_browse_clicked()
{
  auto fileName = QFileDialog::getOpenFileName(this,"Open File",_currentFolder,"Abinit (*.in *.out *_OUT.nc *_HIST *_HIST.nc *_DDB *_DEN *_OPT);;VASP (POSCAR);;CIF (*.cif);;XML (*.xml);;XYZ (*.xyz);; YAML(*.yaml);;All (*)",nullptr,QFileDialog::DontUseNativeDialog);
  if ( !fileName.isEmpty() )
  {
    ui->file->setText(fileName);
    int pos = fileName.lastIndexOf(QRegExp("[/\\\\]"));
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
