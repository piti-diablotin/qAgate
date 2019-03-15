#ifndef QDISPERSION_H
#define QDISPERSION_H

#include <QMainWindow>

namespace Ui {
  class QDispersion;
}

class QDispersion : public QMainWindow
{
  Q_OBJECT

public:
  explicit QDispersion(QWidget *parent = 0);
  ~QDispersion();

private:
  Ui::QDispersion *ui;
};

#endif // QDISPERSION_H
