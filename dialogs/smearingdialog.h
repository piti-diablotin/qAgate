#ifndef SMEARINGDIALOG_H
#define SMEARINGDIALOG_H

#include <QDialog>
#include "base/unitconverter.hpp"

namespace Ui {
  class SmearingDialog;
}

class SmearingDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SmearingDialog(QWidget *parent = 0);
  ~SmearingDialog();

private slots:
  void on_unit_currentIndexChanged(int index);

private:
  Ui::SmearingDialog *ui;
  UnitConverter _unit;
};

#endif // SMEARINGDIALOG_H
