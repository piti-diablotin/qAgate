#include "qtdep.h"
#include "ui_qtdep.h"

qTdep::qTdep(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::qTdep),
    _currentDirectory()
{
    ui->setupUi(this);
    connect(ui->supercell,SIGNAL(openedFile(const QString&)),this,SLOT(updateFilename(const QString&)));
    connect(ui->supercell,SIGNAL(supercellChanged(Dtset)),ui->unitcell,SLOT(dtsetToView(Dtset)));
    connect(ui->unitcell,SIGNAL(unitcellChanged(Dtset)),ui->supercell,SLOT(updateMultiplicity(Dtset)));
    connect(ui->supercell,SIGNAL(rcutHint(double)),ui->options,SLOT(setRcut(double)));
}

qTdep::~qTdep()
{
  delete ui;
}

void qTdep::updateFilename(const QString &filename)
{
  ui->filename->setText(filename);
  ui->toolBox->setCurrentIndex(1);
}

void qTdep::on_buttonBox_clicked(QAbstractButton *button)
{
    QDialogButtonBox::StandardButton b = ui->buttonBox->standardButton(button);
    if ( b == QDialogButtonBox::Save )
    {
    }
    else if ( b == QDialogButtonBox::Open )
    {
      QWidget *widget = ui->toolBox->currentWidget();
      if (dynamic_cast<qTdepOptions*>(widget)!=nullptr)
      {
        QMessageBox::warning(this,tr("Opening file"),tr("You need to select Supercell or Unit cell to open a file"),QMessageBox::Ok);
        return;
      }
      QString filename = QFileDialog::getOpenFileName(this,tr("Open file"),_currentDirectory,"",nullptr,QFileDialog::DontUseNativeDialog);
      if ( !filename.isEmpty() )
      {
        if (dynamic_cast<qTdepSupercell*>(widget)!=nullptr)
        {
          ui->supercell->openFile(filename);
        }
        else if (dynamic_cast<qTdepUnitcell*>(widget)!=nullptr)
        {
          ui->unitcell->openFile(filename);
        }
        else
        {
          QMessageBox::warning(this,tr("Opening file"),tr("You need to select Supercell or Unit cell to open a file"),QMessageBox::Ok);
        }
        filename.truncate(filename.lastIndexOf(QRegExp("\\/")));
        _currentDirectory = filename;
      }
    }
    else if  (b == QDialogButtonBox::Apply )
    {
    }
    else if (b == QDialogButtonBox::Close)
      this->close();
}
