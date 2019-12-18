#ifndef MD_H
#define MD_H

#include <QWidget>
#include "abstracttab.h"

namespace Ui {
  class MD;
}

class MD : public AbstractTab
{
  Q_OBJECT

public:
  explicit MD(QWidget *parent = 0);
  ~MD();

protected:
  virtual void updateStatus(View* view);

private slots:
  void setCombo();

  void selectCombo(PlotWindow::PlotPosition pos, int index, QString name);

  void removeCombo(PlotWindow::PlotPosition pos, int index);

  void on_left_toggled(bool checked);

  void on_right_toggled(bool checked);

  void on_graphCombo_currentIndexChanged(int index);

  void on_angle_clicked();

  void on_plot_activated(int index);

  void on_lattice_clicked();

  void on_gyration_clicked();

  void on_distance_clicked();

  void on_angles_clicked();

  void on_volume_clicked();

  void on_temperature_clicked();

  void on_pressure_clicked();

  void on_entropy_clicked();

  void on_energy_clicked();

  void on_kinetic_clicked();

  void on_stress_clicked();

  void on_thermo_clicked();

  void on_positions_clicked();

  void on_pdf_clicked();

  void on_msd_clicked();

  void on_vacf_clicked();

  void on_pdos_clicked();

  void on_interpolate_clicked();

  public slots:

  void setGraph();

private:
  Ui::MD *ui;
  int _natom;
  double _rmax;
  double _smearing;
  QString _plotCommand;
  int _currentLeft;
  int _currentRight;
};

#endif // MD_H
