#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QLineEdit>

namespace Ui {
  class CommandLine;
}

class CommandLine : public QLineEdit
{
  Q_OBJECT

public:
  explicit CommandLine(QWidget *parent = 0);
  ~CommandLine();

protected:
  virtual void 	keyPressEvent(QKeyEvent *event);

private:
  Ui::CommandLine *ui;

signals:
  void cleared();
  void goBackwards();
  void goForwards();
};

#endif // COMMANDLINE_H
