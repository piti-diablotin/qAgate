#include "pdfdialog.h"
#include "ui_pdfdialog.h"
#include <cmath>

PdfDialog::PdfDialog(double max, double dR, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PdfDialog),
  _unit(UnitConverter::bohr)
{
  ui->setupUi(this);
  ui->rmax->setRange(1.,2*max);
  ui->rmax->setValue(max);
  ui->dr->setRange(1e-3*dR,1e3*dR);
  ui->dr->setValue(dR);
  this->check(0);
  connect(ui->rmax,SIGNAL(valueChanged(double)),this,SLOT(check(double)));
  connect(ui->dr,SIGNAL(valueChanged(double)),this,SLOT(check(double)));
}

PdfDialog::~PdfDialog()
{
  delete ui;
}

double PdfDialog::Rmax() const
{
  return ui->rmax->value()/_unit;
}

double PdfDialog::dR() const
{
  return ui->dr->value()/_unit;
}

void PdfDialog::check(double value)
{
  (void) value;
  double max = ui->rmax->maximum();
  if (ui->rmax->value()>max/2)
    {
      ui->rmax->setStyleSheet("#rmax {color:orange;}");
    }
  else
    {
      ui->rmax->setStyleSheet("#rmax {color:black;}");
    }
  int nbins = std::ceil(ui->rmax->value()/ui->dr->value());
  ui->points->setText(QString::number(nbins));
}

void PdfDialog::on_unit_currentIndexChanged(int index)
{
  (void) index;
  UnitConverter newUnit(ui->unit->unit());
  newUnit.rebase(UnitConverter::bohr);
  double factor = 1/_unit*newUnit;
  double val = ui->rmax->value();
  ui->rmax->setRange(ui->rmax->minimum()*factor,ui->rmax->maximum()*factor);
  ui->rmax->setValue(val*factor);
  val = ui->dr->value();
  ui->dr->setRange(ui->dr->minimum()*factor,ui->dr->maximum()*factor);
  ui->dr->setValue(val*factor);
  _unit = newUnit;
}
