#ifndef ABSTRACTTAB_H
#define ABSTRACTTAB_H

#include <QWidget>
#include "gui/view.h"
#include "dialogs/plotwindow.h"

class AbstractTab : public QWidget
{
  Q_OBJECT

private:

protected:
  bool _autoUpdate;
  static QString _currentFolder;
  static QString _plotCommand;
  static PlotWindow* _plot;

  virtual void updateStatus(View* view)=0;

public:
  explicit AbstractTab(QWidget* parent);
  void update(View* view);
  virtual void plugActions(QWidget* widget);

  static QPlot *plot(PlotWindow::PlotPosition pos = PlotWindow::Left);

signals:
  void sendCommand(QString command,bool=true);
  void plotChanged(QPlot* plot);
  void needCommandLine();
  void statusMessage(QString message,int);

};

#endif // ABSTRACTTAB_H
