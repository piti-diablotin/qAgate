#ifndef STRAINDIALOG_H
#define STRAINDIALOG_H

#include <QDialog>

namespace Ui {
  class StrainDialog;
}

class StrainDialog : public QDialog
{
  Q_OBJECT

public:
  explicit StrainDialog(QWidget *parent = nullptr);
  ~StrainDialog();

  QString currentFolder() const;
  void setCurrentFolder(const QString &currentFolder);
  QString reference() const;
  QString time() const;
  QString only() const;

private slots:
  void on_browse_clicked();

private:
  Ui::StrainDialog *ui;
  QString _currentFolder;
};

#endif // STRAINDIALOG_H
