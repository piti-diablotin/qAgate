#ifndef DUMPDIALOG_H
#define DUMPDIALOG_H

#include <QDialog>

namespace Ui {
  class DumpDialog;
}

class DumpDialog : public QDialog
{
  Q_OBJECT

public:
  explicit DumpDialog(QWidget *parent = 0);
  ~DumpDialog();
  QString filename() const ;
  int step() const;
  QString directory() const;
  void setDirectory(QString dir);

private slots:
  void on_browse_clicked();

  void on_buttonBox_rejected();

private:
  Ui::DumpDialog *ui;
  QString _lastDirectory;
};

#endif // DUMPDIALOG_H
