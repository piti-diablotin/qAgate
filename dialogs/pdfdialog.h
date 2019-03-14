#ifndef PDFDIALOG_H
#define PDFDIALOG_H

#include <QDialog>
#include "base/unitconverter.hpp"

namespace Ui {
  class PdfDialog;
}

class PdfDialog : public QDialog
{
  Q_OBJECT

public:
  explicit PdfDialog(double max, double dR, QWidget *parent = 0);
  ~PdfDialog();
  double Rmax() const;
  double dR() const;

private slots:
  void check(double value);

  void on_unit_currentIndexChanged(int index);

private:
  Ui::PdfDialog *ui;
  UnitConverter _unit;
};

#endif // PDFDIALOG_H
