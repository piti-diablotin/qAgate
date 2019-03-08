#ifndef SPGDIALOG_H
#define SPGDIALOG_H

#include <QDialog>
#include <QDoubleValidator>

namespace Ui {
  class SpgDialog;
}

class SpgDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SpgDialog(QWidget *parent = 0);
  ~SpgDialog();
  double precision() const;

private slots:
  void on_buttonBox_rejected();

private:
  Ui::SpgDialog *ui;
  QDoubleValidator _validator;
  bool _rejected;
};

#endif // SPGDIALOG_H
