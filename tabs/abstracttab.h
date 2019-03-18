#ifndef ABSTRACTTAB_H
#define ABSTRACTTAB_H

#include <QWidget>
#include "gui/view.h"

class AbstractTab : public QWidget
{
  Q_OBJECT

private:

protected:
  bool _autoUpdate;
  virtual void updateStatus(View* view)=0;

public:
  explicit AbstractTab(QWidget* parent);
  void update(View* view);
  virtual void plugActions(QWidget* widget);

signals:
  void sendCommand(QString command,bool=true);
};

#endif // ABSTRACTTAB_H
