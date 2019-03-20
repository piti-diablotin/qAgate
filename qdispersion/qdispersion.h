#ifndef QDISPERSION_H
#define QDISPERSION_H

#include <QMainWindow>
#include <QIntValidator>

namespace Ui {
  class QDispersion;
}

class QDispersion : public QMainWindow
{
  Q_OBJECT

public:
  explicit QDispersion(QWidget *parent = 0);
  ~QDispersion();

private slots:
  void on_nsegments_valueChanged(int arg1);

private:
  Ui::QDispersion *ui;
  QIntValidator _intValidator;

  void setSpacerSize();
};

#endif // QDISPERSION_H
