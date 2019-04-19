#include "qtdepoptions.h"
#include "ui_qtdepoptions.h"
#include <QDoubleValidator>
#include <QRegExpValidator>

qTdepOptions::qTdepOptions(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::qTdepOptions)
{
  ui->setupUi(this);
  QDoubleValidator *dv = new QDoubleValidator(this);
  ui->rcut2->setValidator(dv);
  ui->rcut3->setValidator(dv);
  ui->dosdeltae->setValidator(dv);
  QRegExpValidator *rev = new QRegExpValidator(QRegExp("^\\d+ \\d+ \\d+$"),this);
  ui->ngqpt2->setValidator(rev);
}

qTdepOptions::~qTdepOptions()
{
    delete ui;
}

void qTdepOptions::setRcut(double rcut)
{
    ui->rcut2->setText(QString::number(rcut));
    ui->rcut3->setText(QString::number(rcut));
}

void qTdepOptions::on_order_currentIndexChanged(int index)
{
    bool order3 = (index == 1);
    ui->rcut3->setEnabled(order3);
    ui->rcut3Label->setEnabled(order3);
    ui->rcut3Unit->setEnabled(order3);
}
