#include "distanceunitcombo.h"

DistanceUnitCombo::DistanceUnitCombo(QWidget *parent) :
  unitCombo(parent)
{
  this->addItem("bohr","bohr");
  this->addItem("A","A");
  _unit = UnitConverter::bohr;
}

DistanceUnitCombo::~DistanceUnitCombo()
{

}
