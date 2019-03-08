#ifndef SUPERCELLDIALOG_H
#define SUPERCELLDIALOG_H

#include <QDialog>

namespace Ui {
  class SupercellDialog;
}

class SupercellDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SupercellDialog(QWidget *parent = 0);
  ~SupercellDialog();
  QVector<int> matrix() const;

private slots:
  void on_buttonBox_rejected();
  void validate(QString dummy);

private:
  Ui::SupercellDialog *ui;
  bool _rejected;
};

#endif // SUPERCELLDIALOG_H
