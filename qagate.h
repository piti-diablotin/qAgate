#ifndef QAGATE_H
#define QAGATE_H

#include <QMainWindow>

namespace Ui {
  class qAgate;
}

class qAgate : public QMainWindow
{
  Q_OBJECT

public:
  explicit qAgate(QWidget *parent = 0);
  ~qAgate();

private:
  Ui::qAgate *ui;
};

#endif // QAGATE_H
