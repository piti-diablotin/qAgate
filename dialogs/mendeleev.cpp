#include "mendeleev.h"
#include "ui_mendeleev.h"
#include "base/mendeleev.hpp"
#include "tools/atomicdata.h"
#include <QDebug>
#include <QPalette>
#include <QColorDialog>

Mendeleev::Mendeleev(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Mendeleev),
  _selfSet(false),
  _activeElement(nullptr),
  _massUnit(UnitConverter::amu),
  _radiusUnit(UnitConverter::bohr),
  _rcovUnit(UnitConverter::bohr),
  _modifications(),
  _nModifs(0)
{
  ui->setupUi(this);
}

Mendeleev::~Mendeleev()
{
  delete ui;
}

QStringList Mendeleev::modifications() const
{

  return _modifications;
}

void Mendeleev::build()
{
  _modifications.clear();
  _nModifs = 0;
  auto children = ui->elements->children();
  ui->legend->setColor(
                     QColor(
                       255*MendeTable.color[0][0],
                       255*MendeTable.color[0][1],
                       255*MendeTable.color[0][2]
                     )
        );
  connect(dynamic_cast<atomicData*>(ui->legend),SIGNAL(editElement(atomicData*)),this,SLOT(editElement(atomicData*)));
  for ( auto child = children.begin(); child != children.end(); ++child)
    {
      try
      {
        auto z = Agate::Mendeleev::znucl((*child)->objectName().toStdString());
        atomicData* element = reinterpret_cast<atomicData*>(*child);
        element->set(z,
                     QString::fromStdString(Agate::Mendeleev::name[z]),
                     MendeTable.mass[z],
                     MendeTable.radius[z],
                     MendeTable.rcov[z],
                     QColor(
                       255*MendeTable.color[z][0],
                       255*MendeTable.color[z][1],
                       255*MendeTable.color[z][2]
                     )
                    );
        connect(element,SIGNAL(editElement(atomicData*)),this,SLOT(editElement(atomicData*)));
      }
      catch(...)
      {
        ;
      }
    }
  this->editElement(ui->H);
}

int Mendeleev::result() const
{
  return _nModifs;
}

void Mendeleev::editElement(atomicData* elt)
{
  _selfSet = true;
  _activeElement = elt;
  ui->mass->setValue(elt->mass());
  ui->radius->setValue(elt->radius());
  ui->rcov->setValue(elt->rcov());
  ui->name->setText(elt->name());
  QPalette pal = ui->color->palette();
  pal.setColor(QPalette::Button,elt->color());
  ui->color->setPalette(pal);
  bool enableAll = (elt != ui->legend);
  ui->mass->setEnabled(enableAll);
  ui->radius->setEnabled(enableAll);
  ui->rcov->setEnabled(enableAll);
  ui->name->setEnabled(enableAll);
  _selfSet = false;
}

void Mendeleev::on_mass_valueChanged(double arg1)
{
  if (_selfSet) return;
  //_modifications.push_back(":amu "+_activeElement->name()+" "+QString::number(arg1/_massUnit));
  MendeTable.mass[_activeElement->z()] = arg1/_massUnit;
  ++_nModifs;
  _activeElement->setMass(arg1/_massUnit);
}

void Mendeleev::on_massUnit_currentTextChanged(const QString &arg1)
{
  UnitConverter newUnit = UnitConverter::getFromString(arg1.toStdString());
  newUnit.rebase(UnitConverter::amu);
  double mass = ui->mass->value()/_massUnit*newUnit;
  double minimum = ui->mass->minimum()/_massUnit*newUnit;
  double maximum = ui->mass->maximum()/_massUnit*newUnit;
  _massUnit = newUnit;
  ui->mass->setRange(minimum,maximum);
  _selfSet = true;
  ui->mass->setValue(mass);
  _selfSet = false;
}

void Mendeleev::on_color_clicked()
{
  QColor currentColor = _activeElement->color();
  QColor newColor = QColorDialog::getColor(currentColor,this,tr("Pick a color"),QColorDialog::DontUseNativeDialog);
  if (!newColor.isValid()) return;
  _activeElement->setColor(newColor);
  QPalette pal = ui->color->palette();
  pal.setColor(QPalette::Button,newColor);
  ui->color->setPalette(pal);
  /*
  _modifications.push_back(":color "+_activeElement->name()+" "+
                   QString::number(newColor.red())+" "+
                   QString::number(newColor.green())+" "+
                   QString::number(newColor.blue())
                   );
                   */
  MendeTable.color[_activeElement->z()][0] = newColor.red()/255.f;
  MendeTable.color[_activeElement->z()][1] = newColor.green()/255.f;
  MendeTable.color[_activeElement->z()][2] = newColor.blue()/255.f;
  ++_nModifs;
}

void Mendeleev::on_radius_valueChanged(double arg1)
{
  if (_selfSet) return;
  //_modifications.push_back(":radius "+_activeElement->name()+" "+QString::number(arg1/_radiusUnit));
  MendeTable.radius[_activeElement->z()] = arg1/_radiusUnit;
  ++_nModifs;
  _activeElement->setRadius(arg1/_radiusUnit);
}

void Mendeleev::on_radiusUnit_currentTextChanged(const QString &arg1)
{
  UnitConverter newUnit = UnitConverter::getFromString(arg1.toStdString());
  newUnit.rebase(UnitConverter::bohr);
  double rad = ui->radius->value()/_radiusUnit*newUnit;
  double minimum = ui->radius->minimum()/_radiusUnit*newUnit;
  double maximum = ui->radius->maximum()/_radiusUnit*newUnit;
  _radiusUnit = newUnit;
  ui->radius->setRange(minimum,maximum);
  _selfSet = true;
  ui->radius->setValue(rad);
  _selfSet = false;
}

void Mendeleev::on_rcov_valueChanged(double arg1)
{
  if (_selfSet) return;
  //_modifications.push_back(":rcov "+_activeElement->name()+" "+QString::number(arg1/_rcovUnit));
  MendeTable.rcov[_activeElement->z()] = arg1/_rcovUnit;
  ++_nModifs;
  _activeElement->setRcov(arg1/_rcovUnit);
}

void Mendeleev::on_rcovUnit_currentTextChanged(const QString &arg1)
{
  UnitConverter newUnit = UnitConverter::getFromString(arg1.toStdString());
  newUnit.rebase(UnitConverter::bohr);
  double rad = ui->rcov->value()/_rcovUnit*newUnit;
  double minimum = ui->rcov->minimum()/_rcovUnit*newUnit;
  double maximum = ui->rcov->maximum()/_rcovUnit*newUnit;
  _rcovUnit = newUnit;
  ui->rcov->setRange(minimum,maximum);
  _selfSet = true;
  ui->rcov->setValue(rad);
  _selfSet = false;
}
