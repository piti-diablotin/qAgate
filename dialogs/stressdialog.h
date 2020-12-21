#ifndef STRESSDIALOG_H
#define STRESSDIALOG_H

#include <QDialog>

namespace Ui {
  class StressDialog;
}

class StressDialog : public QDialog
{
  Q_OBJECT

public:
  explicit StressDialog(QWidget *parent = nullptr);
  ~StressDialog();
  
  QString only() const;

private:
  Ui::StressDialog *ui;
};

#endif // STRESSDIALOG_H
