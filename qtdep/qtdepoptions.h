#ifndef QTDEPOPTIONS_H
#define QTDEPOPTIONS_H

#include <QWidget>
#include <bind/tdep.hpp>
#include "base/unitconverter.hpp"

namespace Ui {
  class qTdepOptions;
}

class qTdepOptions : public QWidget
{
  Q_OBJECT

public:
  explicit qTdepOptions(QWidget *parent = 0);
  ~qTdepOptions();
  void setTdep(Tdep &tdep);

private slots:
    void setRcut(double rcut);

    void on_order_currentIndexChanged(int index);

    void on_energyUnit_currentIndexChanged(int index);

signals:
    void energyUnitChanged(UnitConverter::Unit);

private:
  Ui::qTdepOptions *ui;
};

#endif // QTDEPOPTIONS_H
