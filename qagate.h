#ifndef QAGATE_H
#define QAGATE_H

#include <QMainWindow>
#include <QString>

namespace Ui {
  class qAgate;
}

class qAgate : public QMainWindow
{
  Q_OBJECT

public:
  explicit qAgate(QWidget *parent = 0);
  ~qAgate();

private:
  Ui::qAgate *ui;
  bool _tabHiden;

protected slots:
  void manageSignal();
  void manageSignal(QString filename);
  void syncWithUserInput();

signals:
  void emitCommand(QString);
private slots:
  void on_tabWidget_tabBarClicked(int index);
  void on_tabWidget_currentChanged(int index);
};

#endif // QAGATE_H
