#ifndef ANALYZEDIALOG_H
#define ANALYZEDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
  class AnalyzeDialog;
}

class AnalyzeDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AnalyzeDialog(QWidget *parent = 0);
  ~AnalyzeDialog();
  QString normalization();
  QString file();

  QString currentFolder() const;
  void setCurrentFolder(const QString &currentFolder);

private slots:
  void on_browse_clicked();

private:
  Ui::AnalyzeDialog *ui;
  QString _currentFolder;
};

#endif // ANALYZEDIALOG_H
