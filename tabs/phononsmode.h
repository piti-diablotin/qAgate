#ifndef PHONONSMODE_H
#define PHONONSMODE_H

#include <QWidget>
#include <QStandardItemModel>
#include "tabs/abstracttab.h"
#include "canvas/canvasphonons.hpp"

namespace Ui {
  class PhononsMode;
}

class PhononsMode : public AbstractTab
{
  Q_OBJECT

public:
  explicit PhononsMode(QWidget *parent = 0);
  ~PhononsMode();
  void partialUpdate(const CanvasPhonons* canvas);

protected:
  virtual void updateStatus(View* view);

private:
  Ui::PhononsMode *ui;
  QStandardItemModel _qptModel;
  QStandardItemModel _modesModel;
  QModelIndex _currentQptModes;
  bool _doNotUpdate;
  QHash<int,double> _amplitudes;

 private slots:
  void on_qpts_activated(const QModelIndex &index);

  void on_unit_currentIndexChanged(int index);

  void on_modes_activated(const QModelIndex &index);

  void addRemoveQpt(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);

  void addRemoveMode(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);

  void on_qpts_clicked(const QModelIndex &index);

  void on_modes_clicked(const QModelIndex &index);

  void on_all_clicked(bool checked);

  void on_amplitude_valueChanged(double arg1);

  void on_ntime_valueChanged(int arg1);

  void on_amplitudeIndiv_editingFinished();

  void on_reset_clicked();

  void on_ddb_clicked();

  void on_dynamt_clicked();

  void on_open_clicked();

  void on_append_clicked();

  void on_analyze_clicked();

  void on_qpt_clicked();

signals:
  void needPartialUpdate();
  void needFullUpdate();
};

#endif // PHONONSMODE_H
