#ifndef SHIFTDIALOG_H
#define SHIFTDIALOG_H

#include <QDialog>

namespace Ui {
  class ShiftDialog;
}

class ShiftDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ShiftDialog(QWidget *parent = 0);
  ~ShiftDialog();
  void result(double& x, double& y, double& z) const;

private slots:
  void on_buttonBox_rejected();

private:
  Ui::ShiftDialog *ui;
  bool _rejected;
};

#endif // SHIFTDIALOG_H
