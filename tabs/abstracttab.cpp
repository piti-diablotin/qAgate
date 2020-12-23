#include "abstracttab.h"
#include <QDebug>

QPlot *AbstractTab::plot(PlotWindow::PlotPosition pos)
{
  return _plot->currentPlot(pos);
}

AbstractTab::AbstractTab(QWidget* parent) : QWidget(parent),
  _autoUpdate(false)
{

}

void AbstractTab::update(View* view)
{
  _autoUpdate = true;
  this->updateStatus(view);
  _autoUpdate = false;
}

void AbstractTab::plugActions(QWidget* widget)
{
  widget->addActions(this->actions());
}

QString AbstractTab::_currentFolder = ".";
PlotWindow* AbstractTab::_plot = nullptr;
QString AbstractTab::_plotCommand = ":plot ";
