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
  ui->energyUnit->setCurrentIndex(4);
}

qTdepOptions::~qTdepOptions()
{
  delete ui;
}

void qTdepOptions::setTdep(Tdep &tdep)
{
  tdep.order(ui->order->currentText().toUInt());
  tdep.rcut(ui->rcut2->text().toDouble());
  if (ui->order->currentText().toUInt()==3)
    tdep.rcut3(ui->rcut3->text().toDouble());
  tdep.mode(ui->debug->isChecked() ? Tdep::Debug : Tdep::Normal);
  tdep.idealPositions(ui->idealPos->isChecked());
  QStringList ngqpt2str = ui->ngqpt2->text().split(" ");
  int ngqpt2[3];
  ngqpt2[0] = ngqpt2str.at(0).toInt();
  ngqpt2[1] = ngqpt2str.at(1).toInt();
  ngqpt2[2] = ngqpt2str.at(2).toInt();
  auto dosUnit = UnitConverter::getFromString(ui->dosdeltaeUnit->currentData().toString().toStdString());
  dosUnit = UnitConverter::Ha;
  tdep.dosParameters(ngqpt2,ui->dosdeltae->text().toDouble()*dosUnit);

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

void qTdepOptions::on_energyUnit_currentIndexChanged(int index)
{
   emit(energyUnitChanged(UnitConverter::getUnit(ui->energyUnit->itemData(index).toString().toStdString())));
}
