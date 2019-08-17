#include "energyunitcombo.h"

EnergyUnitCombo::EnergyUnitCombo(QWidget *parent) :
  unitCombo(parent)
{
  this->addItem("Ha","Ha");
  this->addItem("eV","eV");
  this->addItem("K","K");
  this->addItem("THz","THz");
  this->addItem("/cm","cm-1");
  _unit = UnitConverter::Ha;
}

EnergyUnitCombo::~EnergyUnitCombo()
{
}

UnitConverter EnergyUnitCombo::currentUnit()
{
 return UnitConverter::getFromString(this->currentData(Qt::UserRole).toString().toStdString());
}
