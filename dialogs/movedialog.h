#ifndef MOVEDIALOG_H
#define MOVEDIALOG_H

#include <QDialog>

namespace Ui {
  class MoveDialog;
}

class MoveDialog : public QDialog
{
  Q_OBJECT

public:
  explicit MoveDialog(QWidget *parent = 0);
  ~MoveDialog();
  void configure(int natom,const double* xred=nullptr);
  void result(int& iatom, double& x, double& y, double& z) const;

private slots:
  void on_iatom_valueChanged(int arg1);

  void on_buttonBox_rejected();

private:
  Ui::MoveDialog *ui;
  QVector<int> _xred;
  bool _rejected;
};

#endif // MOVEDIALOG_H
