#ifndef ATOMDIALOG_H
#define ATOMDIALOG_H

#include <QDialog>

namespace Ui {
  class AtomDialog;
}

class AtomDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AtomDialog(int nToSelect, int natom, QWidget *parent = 0);
  ~AtomDialog();
  QVector<int> atoms();

private slots:
  void on_atom1_valueChanged(int arg1);

  void on_atom2_valueChanged(int arg1);

  void on_atom3_valueChanged(int arg1);

private:
  Ui::AtomDialog *ui;
  int _nToSelect;
  int _natom;
};

#endif // ATOMDIALOG_H
