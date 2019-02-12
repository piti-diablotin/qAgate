#ifndef LOGGER_H
#define LOGGER_H

#include <QWidget>
#include "qostreamcatcher.h"

namespace Ui {
  class Logger;
}

class Logger : public QWidget
{
  Q_OBJECT

public:
  explicit Logger(QWidget *parent = 0);
  ~Logger();

private slots:
  void cout_handle();
  void clog_handle();
  void cerr_handle();
  void on_more_toggled(bool checked);

  void on_debug_toggled(bool checked);

private:
  Ui::Logger *ui;
  bool _isRead;
  QOstreamCatcher _cout;
  QOstreamCatcher _clog;
  QOstreamCatcher _cerr;
  std::streambuf * _oldStreambuf[3];

 signals:
  void debugMode(bool debug);
};

#endif // LOGGER_H
