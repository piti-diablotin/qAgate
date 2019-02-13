#ifndef VIEW_H

#include <QtOpenGL>
#include <QGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QRegExpValidator>
#include "window/window.hpp"

namespace Ui {
  class View;
}

class View : public QGLWidget, protected QOpenGLFunctions, public Window
{
  Q_OBJECT

public:
  explicit View(QWidget *parent = 0);
  ~View();
  virtual void initializeGL();
  virtual void resizeGL(int width, int height);
  virtual void paintGL();
  //virtual void keyPressEvent( QKeyEvent *keyEvent );
  //virtual void keyReleaseEvent( QKeyEvent *keyEvent );
  virtual void mousePressEvent( QMouseEvent *mouseEvent );
  virtual void mouseReleaseEvent( QMouseEvent *mouseEvent );
  virtual void wheelEvent( QWheelEvent *mouseEvent );
  virtual void dragEnterEvent( QDragEnterEvent *dragEnterEvent );
  virtual void dropEvent( QDropEvent *dropEvent );
  virtual void setTitle(const std::string &title);
  virtual void setSize(const int width, const int height);
  const std::map<std::string,bool>& optionBool() const;
  const std::map<std::string,float>& optionFloat() const;
  const std::map<std::string,int>& optionInt() const;

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
  virtual bool userInput(std::stringstream &info);

private slots:
  void timeOut();
  void backInHistory();
  void forwardInHistory();
  void zoomIn();
  void zoomOut();
  void processCommand(QString command, bool pop=true);

  void on_commandFocus_clicked();

  void on_commandLine_returnPressed();

  void on_commandLine_cleared();

public slots:
  void stop();
  void start();
  void setDebugMode(bool debug);

signals:
  void fileOpened(QString);
  void userInput();
  void commandProcessed();
  void updated();
  void closeMe();
  void closeAll();

private:
  Ui::View *ui;
  bool _updateFromTimer;
  QTimer* _timer;
  static const size_t _maxKeys = 14;
  std::array<bool,_maxKeys> _inputKeys;
  double _wheelDelta;
  QRegExpValidator _cmdValidator;
  bool _debug;
};

#endif // VIEW_H
