#ifndef QDOS_H
#define QDOS_H

#include <QMainWindow>
#include "plot/dosdb.hpp"
#include <QStandardItemModel>
#include <QAbstractButton>
#include "base/unitconverter.hpp"
#include "tools/qplot.h"

namespace Ui {
  class QDos;
}

class QDos : public QMainWindow
{
  Q_OBJECT

public:
  explicit QDos(QWidget *parent = 0);
  ~QDos();

  QString currentFolder() const;
  void setCurrentFolder(const QString &currentFolder);

private slots:
  void coordStatusBar(QMouseEvent *event);

  void on_unit_currentIndexChanged(const QString &arg1);

  void on_browse_clicked();

  void on_load_clicked();

  void on_actionAdd_triggered();

  void on_buttonBox_clicked(QAbstractButton *button);

  void on_actionRemove_triggered();

  void on_spin_currentIndexChanged(int index);

  void adjustXRange(const QCPRange &newRange);
  void adjustYRange(const QCPRange &newRange);

private:
  Ui::QDos *ui;
  QString _currentFolder;
  DosDB _db;
  QStandardItemModel _plotCurves;
  UnitConverter _energyUnit;

  void updateAgateCommand();

  void plot();
};

#endif // QDOS_H
