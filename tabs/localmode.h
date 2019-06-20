#ifndef LOCALMODE_H
#define LOCALMODE_H

#include <QWidget>
#include "abstracttab.h"

namespace Ui {
  class LocalMode;
}

class LocalMode : public AbstractTab
{
  Q_OBJECT

public:
  explicit LocalMode(QWidget *parent = 0);
  ~LocalMode();

protected:
  virtual void updateStatus(View* view);

private slots:
  void on_octaAdd_activated(int index);

  void on_octaDel_activated(int index);

  void on_displayGroup_buttonClicked(QAbstractButton* button);

  void on_basisGroup_buttonClicked(QAbstractButton* button);

  void on_colorPlus_clicked();

  void on_colorMinus_clicked();

  void on_exportRotations_clicked();

  void on_exportLengths_clicked();

private:
  Ui::LocalMode *ui;
  QVector<int> _znuclOrder;
  QColor _plusColor;
  QColor _minusColor;
};

#endif // LOCALMODE_H
