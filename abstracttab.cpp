#include "abstracttab.h"

AbstractTab::AbstractTab() :
  _autoUpdate(false)
{

}

void AbstractTab::update(const View &view)
{
  _autoUpdate = true;
  this->updateStatus(view);
  _autoUpdate = false;
}
