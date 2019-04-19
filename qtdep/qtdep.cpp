#include "qtdep.h"
#include "ui_qtdep.h"

qTdep::qTdep(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::qTdep)
{
    ui->setupUi(this);
    connect(ui->supercell,SIGNAL(openedFile(const QString&)),this,SLOT(updateFilename(const QString&)));
    connect(ui->supercell,SIGNAL(supercellChanged(Dtset)),ui->unitcell,SLOT(dtsetToView(Dtset)));
    connect(ui->unitcell,SIGNAL(unitcellChanged(Dtset)),ui->supercell,SLOT(updateMultiplicity(Dtset)));
}

qTdep::~qTdep()
{
  delete ui;
}

void qTdep::updateFilename(const QString &filename)
{
  ui->filename->setText(filename);
}
