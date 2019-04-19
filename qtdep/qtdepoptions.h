#ifndef QTDEPOPTIONS_H
#define QTDEPOPTIONS_H

#include <QWidget>

namespace Ui {
  class qTdepOptions;
}

class qTdepOptions : public QWidget
{
  Q_OBJECT

public:
  explicit qTdepOptions(QWidget *parent = 0);
  ~qTdepOptions();

private slots:
    void setRcut(double rcut);

    void on_order_currentIndexChanged(int index);

private:
  Ui::qTdepOptions *ui;
};

#endif // QTDEPOPTIONS_H
