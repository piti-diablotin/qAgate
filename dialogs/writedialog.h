#ifndef WRITEDIALOG_H
#define WRITEDIALOG_H

#include <QDialog>

namespace Ui {
  class WriteDialog;
}

class WriteDialog : public QDialog
{
  Q_OBJECT

public:
  explicit WriteDialog(QWidget *parent = 0);
  ~WriteDialog();
  QString filename() const ;
  QString directory() const;
  double precision() const;
  QString option() const;
  void setDirectory(QString dir);

private slots:
  void on_browse_clicked();

  void on_buttonBox_rejected();

  void on_current_toggled(bool checked);

  void on_primitive_toggled(bool checked);

  void on_standard_toggled(bool checked);

private:
  Ui::WriteDialog *ui;
  QString _lastDirectory;
};

#endif // WRITEDIALOG_H
