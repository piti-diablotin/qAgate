#ifndef VIEW_H
#define DEFAULT_FONT "/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf"

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include "window/window.hpp"

namespace Ui {
  class View;
}

class View : public QOpenGLWidget, protected QOpenGLFunctions, public Window
{
  Q_OBJECT

public:
  explicit View(QWidget *parent = 0);
  ~View();

protected:
  virtual bool getChar(unsigned key);
  virtual bool getCharPress(unsigned key);
  virtual bool getMouse(unsigned int key) ;
  virtual bool getMousePress(unsigned int key);
  virtual void getWheelOffset(float &wheel);
  virtual void getMousePosition(float &x, float &y);
  virtual void swapBuffers();
  virtual void pollEvents();
  virtual bool exitMainLoop();

private slots:
  void timeOut();

private:
  Ui::View *ui;
  bool _updateFromTimer;
  QTimer* _timer;
};

#endif // VIEW_H
