#include "mendeleev.h"
#include "ui_mendeleev.h"
#include "base/mendeleev.hpp"
#include "atomicdata.h"
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
  _rcovUnit(UnitConverter::bohr)
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
  auto children = ui->elements->children();
  ui->legend->setColor(Qt::lightGray);
  for ( auto child = children.begin(); child != children.end(); ++child)
    {
      try
      {
        auto z = Agate::mendeleev::znucl((*child)->objectName().toStdString());
        atomicData* element = reinterpret_cast<atomicData*>(*child);
        element->set(z,
                     QString::fromStdString(Agate::mendeleev::name[z]),
                     Agate::Mendeleev.mass[z],
                     Agate::Mendeleev.radius[z],
                     Agate::Mendeleev.rcov[z],
                     QColor(
                       255*Agate::Mendeleev.color[z][0],
                       255*Agate::Mendeleev.color[z][1],
                       255*Agate::Mendeleev.color[z][2]
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
  _selfSet = false;
}

void Mendeleev::on_mass_valueChanged(double arg1)
{
  if (_selfSet) return;
  _modifications.push_back(":amu "+_activeElement->name()+" "+QString::number(arg1/_massUnit));
  _activeElement->setMass(arg1/_massUnit);
}

void Mendeleev::on_massUnit_currentIndexChanged(const QString &arg1)
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
  _activeElement->setColor(newColor);
  QPalette pal = ui->color->palette();
  pal.setColor(QPalette::Button,newColor);
  ui->color->setPalette(pal);
  _modifications.push_back(":color "+_activeElement->name()+" "+
                   QString::number(newColor.red())+" "+
                   QString::number(newColor.green())+" "+
                   QString::number(newColor.blue())
                   );
}

void Mendeleev::on_radius_valueChanged(double arg1)
{
  if (_selfSet) return;
  _modifications.push_back(":radius "+_activeElement->name()+" "+QString::number(arg1/_radiusUnit));
  _activeElement->setRadius(arg1/_radiusUnit);
}

void Mendeleev::on_radiusUnit_currentIndexChanged(const QString &arg1)
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
  _modifications.push_back(":rcov "+_activeElement->name()+" "+QString::number(arg1/_rcovUnit));
  _activeElement->setRcov(arg1/_rcovUnit);
}

void Mendeleev::on_rcovUnit_currentIndexChanged(const QString &arg1)
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
