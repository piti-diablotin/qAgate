#include "unitcombo.h"

unitCombo::unitCombo(QWidget *parent) :
  QComboBox(parent),
  _unit()
{
  connect(this,SIGNAL(currentIndexChanged(int)),this,SLOT(rebuildUnit(int)));
}

unitCombo::~unitCombo()
{
}

UnitConverter::Unit unitCombo::unit() const
{
  return _unit;
}

void unitCombo::rebuildUnit(int index)
{
  try{
   _unit = UnitConverter::getUnit(this->itemData(index).toString().toStdString());
   emit(newUnit(_unit));
  }
  catch(...)
  {}
}
