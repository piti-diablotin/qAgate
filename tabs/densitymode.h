#ifndef DENSITYMODE_H
#define DENSITYMODE_H

#include <QWidget>
#include "abstracttab.h"

namespace Ui {
  class DensityMode;
}

class DensityMode : public AbstractTab
{
  Q_OBJECT

public:
  explicit DensityMode(QWidget *parent = 0);
  ~DensityMode();

protected:
  virtual void updateStatus(View* view);

private slots:
  void on_dispdenGroup_buttonClicked(QAbstractButton* button);

  void on_normal_currentIndexChanged(int index);

  void on_function_currentIndexChanged(int index);

  void on_factor_editingFinished();

  void on_colorMin_clicked();

  void on_colorZero_clicked();

  void on_colorMax_clicked();

private:
  Ui::DensityMode *ui;
  QColor _plusColor;
  QColor _minusColor;
  QColor _zeroColor;
};

#endif // DENSITYMODE_H
