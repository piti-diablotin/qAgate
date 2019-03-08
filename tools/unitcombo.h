#ifndef UNITBOX_H
#define UNITBOX_H

#include <QWidget>
#include <QComboBox>
#include "base/unitconverter.hpp"

class unitCombo : public QComboBox
{
  Q_OBJECT

public:
  explicit unitCombo(QWidget *parent = 0);
  ~unitCombo();
  UnitConverter::Unit unit() const;

signals:
  void newUnit(UnitConverter::Unit);

private slots:
  void rebuildUnit(int index);

protected:
  UnitConverter::Unit _unit;
};

#endif // UNITBOX_H
