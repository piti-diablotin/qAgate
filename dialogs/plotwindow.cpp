#include "plotwindow.h"
#include "ui_plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::PlotWindow)
{
  ui->setupUi(this);
  ui->saveLeft->setDefaultAction(ui->actionSaveLeft);
  ui->saveRight->setDefaultAction(ui->actionSaveRight);
  ui->restoreLeft->setDefaultAction(ui->actionRestoreLeft);
  ui->restoreRight->setDefaultAction(ui->actionRestoreRight);
}

PlotWindow::~PlotWindow()
{
  delete ui;
}

QPlot *PlotWindow::currentPlot(PlotPosition pos)
{
  QTabWidget* tabWidget = nullptr;
  switch (pos) {
    case Left:
      tabWidget = ui->left;
      break;
    case Right:
      tabWidget = ui->right;
      break;
  }
  return dynamic_cast<QPlot*>(tabWidget->currentWidget());
}

void PlotWindow::getPlot(PlotWindow::PlotPosition pos, int index, QPlot** plot)
{
  QTabWidget* tabWidget = nullptr;
  switch (pos) {
    case Left:
      tabWidget = ui->left;
      break;
    case Right:
      tabWidget = ui->right;
      break;
    }
  int nTab = tabWidget->count();
  if (index == -1 || index >= nTab)
    {
      QString name = tr("Graph ")+QString::number(nTab);
      *plot = new QPlot(this);
      tabWidget->insertTab(nTab,*plot,name);
      connect(*plot,SIGNAL(newCoordinates(double,double)),this,SLOT(updateStatusBar(double, double)));
      connect(*plot,SIGNAL(beforePlot()),this,SLOT(rise()));
      emit(newTab(pos,nTab,name));
    }
  else
    {
      *plot = dynamic_cast<QPlot*>(tabWidget->widget(index));
    }
  tabWidget->setCurrentWidget(*plot);
}

QVector<QString> PlotWindow::getPlots(PlotWindow::PlotPosition pos)
{
  QTabWidget* tabWidget = nullptr;
  switch (pos) {
    case Left:
      tabWidget = ui->left;
      break;
    case Right:
      tabWidget = ui->right;
      break;
    }
  int nTab = tabWidget->count();
  QVector<QString> names(nTab);
  for (int i = 0; i < nTab; ++i)
    names[i] = tabWidget->tabText(i);
  return names;
}

void PlotWindow::updateStatusBar(double x, double y)
{
  QString info = "x=" + QString::number(x) + " y=" + QString::number(y);
  this->statusBar()->showMessage(info);
}

void PlotWindow::on_left_tabCloseRequested(int index)
{
  if (ui->left->count()<2) return;
  ui->left->removeTab(index);
  emit(removedTab(Left,index));
  emit(newTab(Left,ui->left->currentIndex(),ui->left->tabText(ui->left->currentIndex())));
}

void PlotWindow::on_right_tabCloseRequested(int index)
{
  if (ui->right->count()<2) return;
  ui->right->removeTab(index);
  emit(removedTab(Right,index));
  emit(newTab(Right,ui->right->currentIndex(),ui->right->tabText(ui->right->currentIndex())));
}

void PlotWindow::on_left_tabBarClicked(int index)
{
  emit(newTab(Left,index,ui->left->tabText(index)));
}

void PlotWindow::on_right_tabBarClicked(int index)
{
  emit(newTab(Right,index,ui->right->tabText(index)));
}

void PlotWindow::on_actionSaveLeft_triggered()
{
  dynamic_cast<QPlot*>(ui->left->currentWidget())->save();
}

void PlotWindow::on_actionSaveRight_triggered()
{
  dynamic_cast<QPlot*>(ui->right->currentWidget())->save();
}

void PlotWindow::on_actionRestoreLeft_triggered()
{
  dynamic_cast<QPlot*>(ui->left->currentWidget())->autozoom();
}

void PlotWindow::on_actionRestoreRight_triggered()
{
  dynamic_cast<QPlot*>(ui->right->currentWidget())->autozoom();
}

void PlotWindow::rise()
{
  if (this->isHidden()) this->show();
}
