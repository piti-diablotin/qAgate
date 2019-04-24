#ifndef QTDEP_H
#define QTDEP_H

#include <QMainWindow>
#include <QAbstractButton>
#include "io/eigparser.hpp"
#include <memory>

namespace Ui {
  class qTdep;
}

class qTdep : public QMainWindow
{
  Q_OBJECT

public:
  explicit qTdep(QWidget *parent = 0);
  ~qTdep();
  void openFile(const QString &filename);
  void setSupercellWidget();

private slots:
  void showMessage(const QString string);

  void coordStatusBar(QMouseEvent *event);

  void tdep();

  void updateFilename(const QString &filename);

  void setEnergyUnit(UnitConverter::Unit unit);

  void on_buttonBox_clicked(QAbstractButton *button);

private:
  Ui::qTdep *ui;
  QString _currentDirectory;
  std::unique_ptr<EigParser> _eigparser;
  UnitConverter _energyUnit;

  void replotEigparser();
};

#endif // QTDEP_H
