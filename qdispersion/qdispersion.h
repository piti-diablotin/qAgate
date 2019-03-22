#ifndef QDISPERSION_H
#define QDISPERSION_H

#include <QMainWindow>
#include <QIntValidator>
#include <QRegExpValidator>
#include "io/eigparser.hpp"
#include <memory>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QAbstractButton>

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
  void plot();

private slots:
  void coordStatusBar(QMouseEvent *event);
  void on_nsegments_valueChanged(int arg1);

  void on_fermi_editingFinished();

  void on_fermiUnit_currentIndexChanged(int index);

  void on_energyUnit_currentIndexChanged(int index);

  void on_ignore_valueChanged(int arg1);

  void on_buttonBox_clicked(QAbstractButton *button);

  void on_angular_currentIndexChanged(int index);

  void on_fatbands_clicked(bool checked);

  void on_fermi_valueChanged(double arg1);

private:
  Ui::QDispersion *ui;
  QIntValidator _intValidator;
  QRegExpValidator _atomValidator;
  std::unique_ptr<EigParser> _eigparser;

  void setSpacerSize();
};

#endif // QDISPERSION_H
