#ifndef TYPATDIALOG_H
#define TYPATDIALOG_H

#include <QDialog>

namespace Ui {
  class TypatDialog;
}

class TypatDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TypatDialog(QWidget *parent = 0);
  ~TypatDialog();
  void configure(int natom, const QVector<int>& znucl);
  void result(int& iatom, int& znucl);

private slots:
  void on_buttonBox_rejected();

private:
  Ui::TypatDialog *ui;
  bool _rejected;
};

#endif // TYPATDIALOG_H
