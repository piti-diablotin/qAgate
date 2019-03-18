#ifndef QCONDUCTI_H
#define QCONDUCTI_H

#define _NOEXCEPT noexcept

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QAbstractButton>
#include <QVector>
#include "conducti/conducti.hpp"
#include "base/unitconverter.hpp"
#include "tools/qplot.h"
#include "tools/qostreamcatcher.h"

namespace Ui {
  class QConducti;
}

class QConducti : public QMainWindow
{
  Q_OBJECT

public:
  explicit QConducti(QWidget *parent = 0);
  ~QConducti();
  virtual void dragEnterEvent(QDragEnterEvent *dragEnterEvent);
  virtual void dropEvent(QDropEvent *dropEvent);
  void openFile(const QString &filename);

private slots:
  void updateInfo();

  void plot();

  void coordSigmaStatusBar(QMouseEvent *event);

  void coordHistogramStatusBar(QMouseEvent *event);

  void on_selectionBox_stateChanged(int arg1);

  void on_buttonBox_clicked(QAbstractButton *button);

  void on_energyCombo_currentIndexChanged(const QString &arg1);

  void on_rangeUnit_currentIndexChanged(const QString &arg1);

  void on_smearingUnit_currentIndexChanged(const QString &arg1);

  void on_omin_editingFinished();

  void on_omax_editingFinished();

  void on_bandButton_clicked();

  void on_energyButton_clicked();

  void on_conductiCombo_currentIndexChanged(const QString &arg1);

  void on_selectionBox_toggled(bool checked);

  void sstream_overflowed();
  void sstream_synchronized();
  void sstream_xsputned();

  void on_bfmin_editingFinished();

  void on_bfmax_editingFinished();

  void on_btmin_editingFinished();

  void on_btmax_editingFinished();

  void on_unit_currentIndexChanged(const QString &arg1);

  void on_efmin_editingFinished();

  void on_efmax_editingFinished();

  void on_etmin_editingFinished();

  void on_etmax_editingFinished();

signals:
  void exceptionEmitted(QString,int);

private:
  Ui::QConducti *ui;
  AbiOpt _abiopt;
  Conducti _conducti;
  UnitConverter _units[5];
  Graph::Config _config[6];
  QOstreamCatcher _sstream;
  std::streambuf* _oldStreambuf[3];

  static const int energyUnit = 0;
  static const int omegaUnit = 1;
  static const int smearingUnit = 2;
  static const int energyRangeUnit = 3;
  static const int sigmaUnit = 4;

  void setGraphs();
  void computeAll();
  void setEnergyRange(bool setValue=false);
};

#endif // QCONDUCTI_H
