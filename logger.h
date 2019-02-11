#ifndef LOGGER_H
#define LOGGER_H

#include <QWidget>

namespace Ui {
  class Logger;
}

class Logger : public QWidget
{
  Q_OBJECT

public:
  explicit Logger(QWidget *parent = 0);
  ~Logger();

private:
  Ui::Logger *ui;
};

#endif // LOGGER_H
