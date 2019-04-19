#include "qtdepoptions.h"
#include "ui_qtdepoptions.h"
#include <QDoubleValidator>
#include

qTdepOptions::qTdepOptions(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::qTdepOptions),
{
  ui->setupUi(this);
  QDoubleValidator *dv = new QDoubleValidator(this);
  ui->rcut2->setValidator(dv);
  ui->rcut3->setValidator(dv);
  ui->dosdeltae->setValidator(dv);
}

qTdepOptions::~qTdepOptions()
{
  delete ui;
}
