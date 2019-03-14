#include "atomdialog.h"
#include "ui_atomdialog.h"

AtomDialog::AtomDialog(int nToSelect, int natom, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AtomDialog),
  _nToSelect(nToSelect),
  _natom(natom)
{
  ui->setupUi(this);
  if (_nToSelect==2)
    {
      ui->atom3->hide();
      ui->label3->hide();
    }
  ui->atom1->setRange(1,natom);
  ui->atom2->setRange(1,natom);
  ui->atom3->setRange(1,natom);
}

AtomDialog::~AtomDialog()
{
  delete ui;
}

QVector<int> AtomDialog::atoms()
{
  QVector<int> selected;
  selected.push_back(ui->atom1->value());
  selected.push_back(ui->atom2->value());
  if (_nToSelect==3)
    selected.push_back(ui->atom3->value());
  return selected;
}

void AtomDialog::on_atom1_valueChanged(int arg1)
{
   int atom2 = ui->atom2->value();
   int atom3 = ui->atom3->value();
   if (arg1==atom2)
     {
       ui->atom1->setStyleSheet("#atom1 {color: red;}");
       ui->atom2->setStyleSheet("#atom2 {color: red;}");
     }
   else if (arg1==atom3)
     {
       ui->atom1->setStyleSheet("#atom1 {color: red;}");
       ui->atom3->setStyleSheet("#atom3 {color: red;}");
     }
   else
     {
       ui->atom1->setStyleSheet("#atom1 {color: black;}");
       ui->atom2->setStyleSheet("#atom2 {color: black;}");
       ui->atom3->setStyleSheet("#atom3 {color: black;}");
     }
}

void AtomDialog::on_atom2_valueChanged(int arg1)
{
   int atom1 = ui->atom1->value();
   int atom3 = ui->atom3->value();
   if (arg1==atom1)
     {
       ui->atom2->setStyleSheet("#atom2 {color: red;}");
       ui->atom1->setStyleSheet("#atom1 {color: red;}");
     }
   else if (arg1==atom3)
     {
       ui->atom2->setStyleSheet("#atom2 {color: red;}");
       ui->atom3->setStyleSheet("#atom3 {color: red;}");
     }
   else
     {
       ui->atom1->setStyleSheet("#atom1 {color: black;}");
       ui->atom2->setStyleSheet("#atom2 {color: black;}");
       ui->atom3->setStyleSheet("#atom3 {color: black;}");
     }
}

void AtomDialog::on_atom3_valueChanged(int arg1)
{
   int atom1 = ui->atom1->value();
   int atom2 = ui->atom2->value();
   if (arg1==atom1)
     {
       ui->atom1->setStyleSheet("#atom1 {color: red;}");
       ui->atom3->setStyleSheet("#atom3 {color: red;}");
     }
   else if (arg1==atom2)
     {
       ui->atom2->setStyleSheet("#atom2 {color: red;}");
       ui->atom3->setStyleSheet("#atom3 {color: red;}");
     }
   else
     {
       ui->atom1->setStyleSheet("#atom1 {color: black;}");
       ui->atom2->setStyleSheet("#atom2 {color: black;}");
       ui->atom3->setStyleSheet("#atom3 {color: black;}");
     }
}
