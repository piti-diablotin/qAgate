#include "plandialog.h"
#include "ui_plandialog.h"

PlanDialog::PlanDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PlanDialog)
{
  ui->setupUi(this);
  ui->coordinates->setItemData(0,"cartesian");
  ui->coordinates->setItemData(1,"reduced");
}

PlanDialog::~PlanDialog()
{
  delete ui;
}

QString PlanDialog::plan() const
{
  return ui->plan->currentText();
}

QString PlanDialog::coordinates() const
{
   return ui->coordinates->currentData().toString();
}
