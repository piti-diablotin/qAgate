#ifndef POLARIZATIONDIALOG_H
#define POLARIZATIONDIALOG_H

#include <QDialog>

namespace Ui {
  class PolarizationDialog;
}

class PolarizationDialog : public QDialog
{
  Q_OBJECT

public:
  explicit PolarizationDialog(QWidget *parent = nullptr);
  ~PolarizationDialog();

  QString currentFolder() const;
  void setCurrentFolder(const QString &currentFolder);
  QString ddb() const;

private slots:
  void on_browse_clicked();

private:
  Ui::PolarizationDialog *ui;
  QString _currentFolder;
};

#endif // POLARIZATIONDIALOG_H
