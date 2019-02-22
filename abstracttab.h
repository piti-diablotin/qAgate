#ifndef ABSTRACTTAB_H
#define ABSTRACTTAB_H

#include "view.h"

class AbstractTab
{
private:
  bool _autoUpdate;
protected:
  virtual void updateStatus(const View& view)=0;
public:
  AbstractTab();
  void update(const View &view);
};

#endif // ABSTRACTTAB_H
