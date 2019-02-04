#include "view.h"
#include "ui_view.h"
#include "canvas/canvaspos.hpp"

View::View(QWidget *parent) :
  QOpenGLWidget(parent),
  Window(),
  ui(new Ui::View),
  _updateFromTimer(false),
  _timer(new QTimer(this))
{
  ui->setupUi(this);
  int fps = 60;
  int seconde = 1000; // ms
  int timerInterval = seconde/fps;
  connect(_timer, SIGNAL(timeout()), this, SLOT(timeOut()));
  _timer->start(timerInterval);

  this->setAcceptDrops(true);
  this->setCursor(QCursor(Qt::OpenHandCursor));
  //this->makeCurrent(); ?
  QFont font();

}

View::~View()
{
  delete ui;
  delete _timer;
}

bool View::getChar(unsigned key)
{
  return false;
}

bool View::getCharPress(unsigned key)
{
  return false;
}

bool View::getMouse(unsigned int key)
{
  return false;
}

bool View::getMousePress(unsigned int key)
{
  return false;
}

void View::getWheelOffset(float &wheel)
{
  wheel = 0;
}

void View::getMousePosition(float &x, float &y)
{
  QPoint qpt = QCursor::pos();
  x = (float)qpt.rx();
  y = (float)qpt.ry();
}

void View::swapBuffers()
{

}

void View::pollEvents()
{

}

bool View::exitMainLoop()
{
  return false;
}

void View::timeOut()
{
  _updateFromTimer = true;
}
