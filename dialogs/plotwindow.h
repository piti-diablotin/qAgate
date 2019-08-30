#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QMainWindow>
#include "tools/qplot.h"

namespace Ui {
  class PlotWindow;
}

class PlotWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit PlotWindow(QWidget *parent = 0);
  ~PlotWindow();
  enum PlotPosition {Left, Right};
  QPlot *currentPlot(PlotPosition pos);

public slots:
  void getPlot(PlotPosition pos, int index, QPlot** plot);
  QVector<QString> getPlots(PlotPosition pos);

signals:
  void newTab(PlotWindow::PlotPosition pos,int index,QString name);
  void removedTab(PlotWindow::PlotPosition pos,int index);

private slots:
  void updateStatusBar(double x, double y);

  void on_left_tabCloseRequested(int index);

  void on_right_tabCloseRequested(int index);

  void on_left_tabBarClicked(int index);

  void on_right_tabBarClicked(int index);

  void on_actionSaveLeft_triggered();

  void on_actionSaveRight_triggered();

  void on_actionRestoreLeft_triggered();

  void on_actionRestoreRight_triggered();

  void rise();

private:
  Ui::PlotWindow *ui;
};

#endif // PLOTWINDOW_H
