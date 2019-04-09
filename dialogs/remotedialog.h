#ifndef REMOTEDIALOG_H
#define REMOTEDIALOG_H

#include <QDialog>
#include <QIntValidator>
#include <QRegExpValidator>
#include "base/ssh.hpp"
#include "tools/qostreamcatcher.h"

namespace Ui {
  class RemoteDialog;
}

class RemoteDialog : public QDialog
{
  Q_OBJECT

public:
  explicit RemoteDialog(QWidget *parent = 0);
  ~RemoteDialog();
  void removeAppend(bool remove);

private slots:
  void on_browse_clicked();

  void on_test_clicked();

  void on_get_clicked();

  void handle_stream();

  void on_open_clicked();

  void on_append_clicked();

  void need_to_retest(QString text);

private:
  Ui::RemoteDialog *ui;
  QIntValidator _intValidator;
  QRegExpValidator _textValidator;
  QOstreamCatcher _stream;
  std::string _filename;

  bool connectSsh(Ssh &ssh);

signals:
  void sendCommand(QString,bool=true);

};

#endif // REMOTEDIALOG_H
