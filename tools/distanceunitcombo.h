#ifndef DISTANCEUNITCOMBO_H
#define DISTANCEUNITCOMBO_H

#include <QWidget>
#include "unitcombo.h"

namespace Ui {
  class DistanceUnitCombo;
}

class DistanceUnitCombo : public unitCombo
{
  Q_OBJECT

public:
  explicit DistanceUnitCombo(QWidget *parent = 0);
  ~DistanceUnitCombo();

};

#endif // DISTANCEUNITCOMBO_H
