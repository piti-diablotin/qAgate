#ifndef MULTIBINIT_H
#define MULTIBINIT_H

#include <QWidget>
#include "tabs/abstracttab.h"

namespace Ui {
  class Multibinit;
}

class Multibinit : public AbstractTab
{
    Q_OBJECT

  public:
    explicit Multibinit(QWidget *parent = 0);
    ~Multibinit();

protected:
  virtual void updateStatus(View* view);

  private slots:
    void on_seedType_currentIndexChanged(int index);

    void on_instable_currentIndexChanged(int index);

    void on_generate_clicked();

  private:
    Ui::Multibinit *ui;
};

#endif // MULTIBINIT_H
