#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include "abstracttab.h"
#include "dialogs/dumpdialog.h"
#include "dialogs/writedialog.h"

namespace Ui {
  class Home;
}

class Home : public AbstractTab
{
  Q_OBJECT

public:
  explicit Home(QWidget *parent = 0);
  ~Home();
  void setDistance(const double distance);
  void setAngle(const double angle);
  void getAngleAtoms(int& atom1, int& atom2, int& atom3);
  void getDistanceAtoms(int& atom1, int& atom2);

protected:
  virtual void updateStatus(View* view);

private slots:
  void on_update_clicked();

  void on_dumpHist_clicked();

  void on_dumpXyz_clicked();

  void on_saveAbinit_clicked();

  void on_savePoscar_clicked();

  void on_saveCif_clicked();

  void on_load_clicked();

  void on_distanceCheckBox_clicked(bool checked);

  void on_angleCheckBox_clicked(bool checked);


  void on_distanceUnit_currentTextChanged(const QString &arg1);

  void on_distanceAtom1_valueChanged(int arg1);

  void on_distanceAtom2_valueChanged(int arg1);

  void on_angleAtom1_valueChanged(int arg1);

  void on_angleAtom2_valueChanged(int arg1);

  void on_angleAtom3_valueChanged(int arg1);

  void on_spg_clicked();

private:
  Ui::Home *ui;
  DumpDialog _dumpDialog;
  WriteDialog _writeDialog;
  int _natom;
  double _distance;
  UnitConverter _distanceUnit;

signals:
  void needDistance(bool);
  void needAngle(bool);
};

#endif // HOME_H
