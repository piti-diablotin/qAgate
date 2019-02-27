#include "abstracttab.h"
#include <QDebug>

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
