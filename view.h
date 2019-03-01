#ifndef VIEW_H
#define VIEW_H

#include <QtGlobal>
#include <QtOpenGL>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
#include <QOpenGLWidget>
#else
#include <QGLWidget>
#endif

#include <QOpenGLFunctions>
#include <QTimer>
#include <QRegExpValidator>
#include "window/window.hpp"

namespace Ui {
  class View;
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
class View : public QOpenGLWidget, protected QOpenGLFunctions, public Window
#else
class View : public QGLWidget, protected QOpenGLFunctions, public Window
#endif
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
  template<typename T>
  T option(QString key);
  void imageSaverInfo(ImageSaver::ImageType& format, int& quality, ImageSuffix& suffix) const;
  unsigned getDisplay() const;
  const Canvas* getCanvas();

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
  void mouseInput();
  void commandProcessed();
  void updated();
  void closeMe();
  void closeAll();
  void newSize(int width,int height);

private:
  Ui::View *ui;
  bool _updateFromTimer;
  QTimer* _timer;
  static const size_t _maxKeys = 14;
  std::array<bool,_maxKeys> _inputKeys;
  double _wheelDelta;
  QRegExpValidator _cmdValidator;
  bool _debug;
  bool _fromCommandLine;
};

#endif // VIEW_H
