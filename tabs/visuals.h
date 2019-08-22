#ifndef VISUALS_H
#define VISUALS_H

#include <QWidget>
#include "abstracttab.h"

namespace Ui {
  class Visuals;
}

class Visuals : public AbstractTab
{
  Q_OBJECT

public:
  explicit Visuals(QWidget *parent = 0);
  ~Visuals();

protected:
  virtual void updateStatus(View* view);

private:
  Ui::Visuals *ui;
  QVector<int> _znuclOrder;
  QColor _octaColor;

signals:
  void switchAngles();
  void switchTimeInfo();
  void translatePX();
  void translateMX();
  void translatePY();
  void translateMY();
  void translatePZ();
  void translateMZ();
  void alongX();
  void alongY();
  void alongZ();
private slots:
  void on_actionAngles_triggered();
  void on_actionTime_triggered();
  void on_axis_clicked();
  void on_action_px_triggered();
  void on_action_py_triggered();
  void on_action_pz_triggered();
  void on_action_mz_triggered();
  void on_action_my_triggered();
  void on_action_mx_triggered();
  void on_actionX_triggered();
  void on_actionY_triggered();
  void on_actionZ_triggered();
  void on_psi_valueChanged(int arg1);

  void on_theta_valueChanged(int arg1);

  void on_phi_valueChanged(int arg1);

  void on_displayGroup_buttonClicked(QAbstractButton* button);

  void on_optionsGroup_buttonClicked(QAbstractButton* button);

  void on_spin_currentIndexChanged(int index);

  void on_octaAdd_activated(int index);

  void on_octaDel_activated(int index);

  void on_atoms_clicked(bool checked);

  void on_octaColor_clicked();

  void on_spinRelative_clicked(bool checked);

public slots:
  void updateAngles(View* view);
};

#endif // VISUALS_H
