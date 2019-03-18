#ifndef MENDELEEV_H
#define MENDELEEV_H

#include <QDialog>
#include <QStringList>
#include "tools/atomicdata.h"
#include "base/unitconverter.hpp"

namespace Ui {
  class Mendeleev;
}

class Mendeleev : public QDialog
{
  Q_OBJECT

public:
  explicit Mendeleev(QWidget *parent = 0);
  ~Mendeleev();
  QStringList modifications() const;
  void build();

public slots:
  void editElement(atomicData* elt);

private slots:
  void on_mass_valueChanged(double arg1);

  void on_massUnit_currentIndexChanged(const QString &arg1);

  void on_color_clicked();

  void on_radius_valueChanged(double arg1);

  void on_radiusUnit_currentIndexChanged(const QString &arg1);

  void on_rcov_valueChanged(double arg1);

  void on_rcovUnit_currentIndexChanged(const QString &arg1);


private:
  Ui::Mendeleev *ui;
  bool _selfSet;
  atomicData* _activeElement;
  UnitConverter _massUnit;
  UnitConverter _radiusUnit;
  UnitConverter _rcovUnit;
  QStringList _modifications;
};

#endif // MENDELEEV_H
