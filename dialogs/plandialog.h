#ifndef PLANDIALOG_H
#define PLANDIALOG_H

#include <QDialog>

namespace Ui {
  class PlanDialog;
}

class PlanDialog : public QDialog
{
  Q_OBJECT

public:
  explicit PlanDialog(QWidget *parent = 0);
  ~PlanDialog();
  QString plan() const;
  QString coordinates() const;

private:
  Ui::PlanDialog *ui;
};

#endif // PLANDIALOG_H
