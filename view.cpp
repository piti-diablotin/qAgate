#include "view.h"
#include "ui_view.h"
#include "canvas/canvaspos.hpp"
#include <QMessageBox>
#include <QDebug>

View::View(QWidget *parent) :
  QOpenGLWidget(parent),
  Window(),
  ui(new Ui::View),
  _updateFromTimer(false),
  _timer(new QTimer(this)),
  _inputKeys({false}),
  _wheelDelta(0),
  _cmdValidator(QRegExp("^:.*$"),this)
{
  ui->setupUi(this);
  ui->commandLine->setValidator(&_cmdValidator);
  int fps = 60;
  int seconde = 1000; // ms
  int timerInterval = seconde/fps;
  connect(_timer, SIGNAL(timeout()), this, SLOT(timeOut()));
  _timer->start(timerInterval);

  this->setAcceptDrops(true);
  this->setCursor(QCursor(Qt::OpenHandCursor));
  //this->makeCurrent(); ?
  _render._doRender = false;
  _mouseButtonLeft = 0;
  _mouseButtonRight = 1;
  _mouseButtonMiddle = 2;
  connect(ui->commandLine,SIGNAL(goBackwards()),this,SLOT(backInHistory()));
  connect(ui->commandLine,SIGNAL(goForwards()),this,SLOT(forwardInHistory()));
}

View::~View()
{
  delete ui;
}

void View::initializeGL()
{
  Window::beginGl();
  _arrow.reset(new TriArrow(true));
  _canvas.reset(new CanvasPos(true));
  this->setUpdateBehavior(QOpenGLWidget::PartialUpdate);
}

void View::resizeGL(int width, int height)
{
  _width = width;
  _height = height;
}

void View::paintGL()
{
  if ( !_updateFromTimer ) _optioni["initBuffer"] = 0;
    Window::loopStep();
    _updateFromTimer = false;
    if ( _optionb["updated"] )
      {
        if (_canvas->ntime() > 1)
          _canvas->isPaused() ?
                ui->upperLeft->setText("Speed:x"+QString::number(_optionf["speed"])+", Paused"):
            ui->upperLeft->setText("Speed:x"+QString::number(_optionf["speed"]));

        emit(updated());
      }
}

/*
void View::keyPressEvent(QKeyEvent *keyEvent)
{
  if (keyEvent->key() == Qt::Key_Colon) ui->commandLine->setFocus();
}
*/

bool View::getChar(unsigned key)
{
  (void) key;
  return false;
}

bool View::getCharPress(unsigned key)
{
  (void) key;
  return false;
}

bool View::getMouse(unsigned int key)
{
  if (key >= _maxKeys) return false;
  bool pressed = _inputKeys[key];
  _inputKeys[key] = false;
  return pressed;
}

bool View::getMousePress(unsigned int key)
{
  if (key >= _maxKeys) return false;
  bool pressed = _inputKeys[key];
  return pressed;
}

void View::getWheelOffset(float &wheel)
{
  wheel = (float) _wheelDelta;
  _wheelDelta = 0;
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

bool View::userInput(std::stringstream &info)
{
  bool newAction = Window::userInput(info);
  if (_mode == mode_process) {
      ui->commandLine->setReadOnly(true);
      ui->commandLine->setText(QString::fromStdString(_command));
      emit(commandProcessed());
      ui->commandLine->setStyleSheet(
            "#commandLine { color:rgb("+QString::number(_render._color[0])+","
          + QString::number(_render._color[1])+","
          + QString::number(_render._color[2])+"); background-color:rgba("+QString::number(_background[0])+","
          + QString::number(_background[1])+","
          + QString::number(_background[2])+",0); }");
      ui->infoLine->setStyleSheet(
            "#infoLine { color:rgb("+QString::number(_render._color[0])+","
          + QString::number(_render._color[1])+","
          + QString::number(_render._color[2])+"); }");
      ui->upperLeft->setStyleSheet(
            "#upperLeft { color:rgb("+QString::number(_render._color[0])+","
          + QString::number(_render._color[1])+","
          + QString::number(_render._color[2])+"); }");
      ui->commandFocus->setStyleSheet(
            "#commandFocus { background-color:rgba("+QString::number(_background[0])+","
          + QString::number(_background[1])+","
          + QString::number(_background[2])+",0); }");
    }
  if (newAction) emit(userInput());
  ui->infoLine->setText(QString::fromStdString(info.str()));
  return newAction;
}

void View::timeOut()
{
  if (_optioni["shouldExit"] == 1)
    {
      emit(closeMe());
    }
  else if (_optioni["shouldExist"] == 2)
    {
      emit(closeAll());
    }
  _updateFromTimer = true;
  this->update();
}

void View::backInHistory()
{
  if ( (_commandStackNo-1) < _commandStack.size()) { // _commandStackNo is unsigend so if <0 it is apriori >> _commandStack.size()
    auto tmp = _commandStack[--_commandStackNo];
    ui->commandLine->setText(QString::fromStdString(tmp.substr(0,tmp.size()-1)));
    }
}

void View::forwardInHistory()
{
  if ( (_commandStackNo+1) < _commandStack.size() ) {
    auto tmp = _commandStack[++_commandStackNo];
    ui->commandLine->setText(QString::fromStdString(tmp.substr(0,tmp.size()-1)));
    }
}

void View::zoomIn()
{
  _wheelDelta = -1;
}

void View::zoomOut()
{
  _wheelDelta = 1;
}

void View::processCommand(QString command, bool pop)
{
  if ( pop )
    while (_inputChar.size() > 0 ) _inputChar.pop();
  for ( int i = 0 ; i < command.size() ; ++i )
    _inputChar.push((unsigned int)command[i].toLatin1());
  if (command.startsWith(':')) _inputChar.push((unsigned int)'\n');
  if (_debug)
    {
      std::clog << command.toStdString() << std::endl;
    }
}

void View::mousePressEvent( QMouseEvent *mouseEvent ) {
  switch( mouseEvent->button() )
    {
    case Qt::LeftButton:
      _inputKeys[_mouseButtonLeft] = true;
      this->setCursor(QCursor(Qt::ClosedHandCursor));
      break;
    case Qt::RightButton:
      _inputKeys[_mouseButtonRight] = true;
      this->setCursor(QCursor(Qt::SizeAllCursor));
      break;
      //case Qt::MiddleButton:
      //  _inputKeys[_mouseButtonMiddle] = true;
      //  break;
    case Qt::NoButton:
    default:
      break;
    }
}

void View::mouseReleaseEvent( QMouseEvent *mouseEvent ) {
  switch( mouseEvent->button() )
    {
    case Qt::LeftButton:
      _inputKeys[_mouseButtonLeft] = false;
      break;
    case Qt::RightButton:
      _inputKeys[_mouseButtonRight] = false;
      break;
      //case Qt::MiddleButton:
      //  _inputKeys[_mouseButtonMiddle] = false;
      //  break;
      //case Qt::NoButton:
    default:
      break;
    }
  this->setCursor(QCursor(Qt::OpenHandCursor));
}

void View::wheelEvent(QWheelEvent *mouseEvent)
{
  _wheelDelta = (double)mouseEvent->delta()/120.;
}

void View::dragEnterEvent( QDragEnterEvent *dragEnterEvent ) {
  if (dragEnterEvent->mimeData()->hasFormat("text/uri-list"))
    dragEnterEvent->acceptProposedAction();
}

void View::dropEvent(QDropEvent *dropEvent)
{
  if ( _canvas == nullptr ) return;

  QList<QUrl> urls = dropEvent->mimeData()->urls();
  if (urls.isEmpty())
    return;

  QString fileName = urls.first().toLocalFile();
  if (fileName.isEmpty())
    return;

  QString command = ":open "+fileName;
  this->processCommand(command);
  /*
  try
  {
    _canvas->openFile(fileName.toStdString());
    size_t pos = fileName.toStdString().find_last_of("/\\");
    this->setTitle(fileName.toStdString().substr(pos+1));
  }
  catch (Exception& e)
  {
    e.ADD("Ignoring file "+fileName.toStdString(), ERRWAR);
    QMessageBox::warning(this,tr("Warning"),QString::fromStdString(e.fullWhat()));
  }
  */

  for (auto url = urls.begin()+1; url != urls.end(); ++url)
    {
      QString fileName = url->toLocalFile();
      if (!fileName.isEmpty())
        {
          QString command = ":append "+fileName;
          this->processCommand(command);
          /*
          try
          {
            _canvas->appendFile(fileName.toStdString());
          }
          catch (Exception& e)
          {
            e.ADD("Ignoring file "+fileName.toStdString(), ERRWAR);
            QMessageBox::warning(this,tr("Warning"),QString::fromStdString(e.fullWhat()));
          }
          */
        }
    }
}

void View::setTitle(const std::string &title)
{
  _title = title;
  emit(fileOpened(QString::fromStdString(title)));
}

void View::setSize(const int width, const int height)
{
  if (width==_width && height==_height) return;
  int dw = width-_width;
  int dh = height-_height;
  QWidget* main = this;
  while (main->parentWidget() != nullptr)
    main = main->parentWidget();
  auto msize = main->size();
  msize+=QSize(dw,dh);
  main->resize(msize.width(),msize.height());
}

const std::map<std::string, bool> &View::optionBool() const {return _optionb;}

const std::map<std::string, float> &View::optionFloat() const  {return _optionf;}

const std::map<std::string, int> &View::optionInt() const {return _optioni;}

void View::on_commandFocus_clicked()
{
  if (ui->commandLine->isReadOnly()) ui->commandLine->clear();
  ui->commandLine->setReadOnly(false);
  ui->commandLine->setFocus();
  ui->commandLine->setText(":");
  _commandStackNo = _commandStack.size();
}

void View::on_commandLine_returnPressed()
{
  QString command = ui->commandLine->text();
  this->processCommand(command);
  ui->commandLine->clear();
}

void View::on_commandLine_cleared()
{
  ui->commandLine->setReadOnly(true);
  ui->commandLine->setText(QString::fromStdString(_command));
}

void View::stop() { _timer->stop(); while ( !_inputChar.empty() ) _inputChar.pop();}

void View::start() { _timer->start();}

void View::setDebugMode(bool debug)
{
  _debug = debug;
}
