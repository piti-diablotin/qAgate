#ifndef PHONONSMODE_H
#define PHONONSMODE_H

#include <QWidget>
#include "tabs/abstracttab.h"

namespace Ui {
  class PhononsMode;
}

class PhononsMode : public AbstractTab
{
  Q_OBJECT

public:
  explicit PhononsMode(QWidget *parent = 0);
  ~PhononsMode();

protected:
  virtual void updateStatus(View* view);

private:
  Ui::PhononsMode *ui;
};

#endif // PHONONSMODE_H
