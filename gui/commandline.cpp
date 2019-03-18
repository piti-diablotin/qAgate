#include "commandline.h"
#include "ui_commandline.h"
#include <QKeyEvent>

CommandLine::CommandLine(QWidget *parent) :
  QLineEdit(parent),
  ui(new Ui::CommandLine)
{
  ui->setupUi(this);
}

CommandLine::~CommandLine()
{
  delete ui;
}

void CommandLine::keyPressEvent(QKeyEvent *event)
{
  switch(event->key())
    {
    case Qt::Key_Escape:
      this->clearFocus();
      this->clear();
      emit(cleared());
      break;
    case Qt::Key_Up:
      emit(goBackwards());
      break;
    case Qt::Key_Down:
      emit(goForwards());
      break;
    default:
      QLineEdit::keyPressEvent(event);
    }
}
