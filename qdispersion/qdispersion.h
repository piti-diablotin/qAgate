#ifndef QDISPERSION_H
#define QDISPERSION_H

#include <QMainWindow>
#include <QIntValidator>
#include "io/eigparser.hpp"
#include <memory>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace Ui {
  class QDispersion;
}

class QDispersion : public QMainWindow
{
  Q_OBJECT

public:
  explicit QDispersion(QWidget *parent = 0);
  ~QDispersion();
  virtual void dragEnterEvent(QDragEnterEvent *dragEnterEvent);
  virtual void dropEvent(QDropEvent *dropEvent);
  void openFile(const QString &filename);
  void plot() const;

private slots:
  void coordStatusBar(QMouseEvent *event);
  void on_nsegments_valueChanged(int arg1);

private:
  Ui::QDispersion *ui;
  QIntValidator _intValidator;
  std::unique_ptr<EigParser> _eigparser;

  void setSpacerSize();
};

#endif // QDISPERSION_H
