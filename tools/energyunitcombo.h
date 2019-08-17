#ifndef ENERGYUNITCOMBO_H
#define ENERGYUNITCOMBO_H

#include <QWidget>
#include "unitcombo.h"

namespace Ui {
  class EnergyUnitCombo;
}

class EnergyUnitCombo : public unitCombo
{
  Q_OBJECT

public:
  explicit EnergyUnitCombo(QWidget *parent = 0);
  ~EnergyUnitCombo();
  UnitConverter currentUnit();
};

#endif // ENERGYUNITCOMBO_H
