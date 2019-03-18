#ifndef ATOMICDATA_H
#define ATOMICDATA_H

#include <QWidget>
#include <QColor>
#include <QMouseEvent>

namespace Ui {
  class atomicData;
}

class atomicData : public QWidget
{
  Q_OBJECT

public:
  explicit atomicData(QWidget *parent = 0);
  ~atomicData();
  void set(int z, QString name, double mass, double radius, double rcov, QColor color);
  int z() const;
  QString name() const;
  double mass() const;
  double radius() const;
  double rcov() const;
  QColor color() const;

public slots:
  virtual void mousePressEvent(QMouseEvent* mouseEvent);
  void setMass(double mass);
  void setRadius(double radius);
  void setRcov(double rcov);
  void setColor(QColor color);

signals:
  void editElement(atomicData*);

private:
  Ui::atomicData *ui;
  double _mass;
  double _radius;
  double _rcov;
  QColor _color;
};

#endif // ATOMICDATA_H
