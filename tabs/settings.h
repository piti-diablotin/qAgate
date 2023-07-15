#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "gui/view.h"
#include "tabs/abstracttab.h"
#include "dialogs/mendeleev.h"

namespace Ui {
  class Settings;
}

class Settings : public AbstractTab
{
  Q_OBJECT

public:
  explicit Settings(QWidget *parent = 0);
  ~Settings();

protected:
  virtual void updateStatus(View* view);

private:
  Ui::Settings *ui;
  UnitConverter _bondRadiusUnit;
  Mendeleev _mendeleev;

public slots:
  void updateDisplaySize(int width, int height);

signals:
  void switchFilling();
  void switchLight();
  void switchPerspective();
  void switchAA();
  void switchAngles();
  void switchTimeInfo();
private slots:
  void on_actionPerspective_triggered();
  void on_actionLight_triggered();
  void on_actionFilling_triggered();
  void on_actionAA_triggered();
  void on_ndiv_valueChanged(int arg1);
  void on_format_currentTextChanged(const QString &arg1);
  void on_suffix_currentTextChanged(const QString &arg1);
#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
  void on_quality_valueChanged(const QString &arg1);
  void on_size_valueChanged(const QString &arg1);
  void on_ndiv_valueChanged(const QString &arg1);
#else
  void on_quality_textChanged(const QString &arg1);
  void on_size_textChanged(const QString &arg1);
  void on_ndiv_textChanged(const QString &arg1);
#endif
  void on_background_clicked();
  void on_foreground_clicked();
  //void on_width_valueChanged(const QString &arg1);
  //void on_height_valueChanged(const QString &arg1);
  void on_width_editingFinished();
  void on_height_editingFinished();
  void on_bond_valueChanged(double arg1);
  void on_bondRadius_valueChanged(double arg1);
  void on_comboBox_newUnit(UnitConverter::Unit unit);
  void on_actionMendeleev_triggered();
  void updateMendeleev();
  void on_up_clicked();
  void on_down_clicked();
  void on_perspectiveAngle_valueChanged(int arg1);
  void on_perspective_toggled(bool checked);
};

#endif // SETTINGS_H
