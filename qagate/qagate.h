#ifndef QAGATE_H
#define QAGATE_H

#include <QMainWindow>
#include <QString>
#include "tools/qplot.h"

namespace Ui {
  class qAgate;
}

class qAgate : public QMainWindow
{
  Q_OBJECT

public:
  explicit qAgate(QWidget *parent = 0);
  ~qAgate();
  void initInput(int argc, const char** argv);
  void setParameters(const std::string &config);

private:
  Ui::qAgate *ui;
  bool _tabHiden;
  bool _homeNeedsDistance;
  bool _homeNeedsAngle;
  QButtonGroup* _canvasButtons;
  void updateNeeds();

protected slots:
  void manageSignal();
  void manageSignal(QString filename);
  void syncWithUserInput();
  void updateTab();
  void setNeeds(bool need=true);
  void setPlot(QPlot* plot);
  void updateCanvas(QAbstractButton* button);

signals:
  void emitCommand(QString,bool pop=true);
private slots:
  void on_tabWidget_tabBarClicked(int index);
  void on_tabWidget_currentChanged(int index);
};

#endif // QAGATE_H
