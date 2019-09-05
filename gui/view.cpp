#include "view.h"
#include "ui_view.h"
#include "canvas/canvaspos.hpp"
#include <QMessageBox>
#include <QDebug>

View::View(QWidget *parent) :
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
  QOpenGLWidget(parent),
#else
  QGLWidget(parent),
#endif
  Window(),
  ui(new Ui::View),
  _updateFromTimer(false),
  _timer(new QTimer(this)),
  _inputKeys({false}),
  _wheelDelta(0),
  _cmdValidator(QRegExp("^:.*$"),this),
  _fromCommandLine(false)
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
  _canvas.reset(new CanvasPos(true));
}

View::~View()
{
  delete ui;
}

void View::initializeGL()
{
  this->makeCurrent();
  initializeOpenGLFunctions();
  Window::beginGl();
  _arrow.reset(new TriArrow(true));
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
  this->setUpdateBehavior(QOpenGLWidget::PartialUpdate);
#endif
}

void View::resizeGL(int width, int height)
{
  _width = width;
  _height = height;
  emit(newSize(_width,_height));
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
  if (newAction && _modeMouse != mode_mouse && _fromCommandLine) emit(userInput());
  if (newAction && _modeMouse == mode_mouse) emit(mouseInput());
  if (_inputChar.empty()) _fromCommandLine = false;
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
  if (_optionb["debug"])
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
      ui->commandFocus->setFocus();
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
      this->processCommand(command,false);
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
  _image.setBasename(_title);
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
  emit(newSize(_width,_height));
}

template<>
bool View::option(QString key)
{
  try
  {

    return _optionb[key.toStdString()];
  }
  catch (...)
  {
    throw EXCEPTION("key " + key.toStdString()+ " does not exit",ERRDIV);
  }
}

template<>
float View::option(QString key)
{
  try
  {
    return _optionf[key.toStdString()];
  }
  catch (...)
  {
    throw EXCEPTION("key " + key.toStdString()+ " does not exit",ERRDIV);
  }
}

template<>
int View::option(QString key)
{
  auto it = _optioni.find(key.toStdString());
  if (it == _optioni.end() )
    throw EXCEPTION("key " + key.toStdString()+ " does not exit",ERRDIV);
  else
    return it->second;
  return 0;
}

void View::imageSaverInfo(ImageSaver::ImageType &format, int &quality, Window::ImageSuffix &suffix) const
{
  format = _image.getFormat();
  quality = _image.getQuality();
  suffix = _imageSuffixMode;
}

unsigned View::getDisplay() const
{
  if (dynamic_cast<CanvasPos*>(_canvas.get()))
    return dynamic_cast<CanvasPos*>(_canvas.get())->getDisplay();
  return 0;
}

const Canvas *View::getCanvas() const
{
  return _canvas.get();
}

double View::getAngle(int atom1, int atom2, int atom3) const
{
  try {
    if (_canvas->histdata()!=nullptr)
    {
      return _canvas->histdata()->getAngle(atom1,atom2,atom3,_canvas->itime());
    }
    else
      return 0;
  }
  catch (...)
  {
    return 0;
  }
  return 0;
}

double View::getDistance(int atom1, int atom2) const
{
  try {
    if (_canvas->histdata()!=nullptr)
    {
      return _canvas->histdata()->getDistance(atom1,atom2,_canvas->itime());
    }
    else
      return 0;
  }
  catch (...)
  {
    return 0;
  }
  return 0;
}

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
  _fromCommandLine = true;
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
  _optionb["debug"] = debug;
}

void View::setFromCommandLine(bool fromCommandLine)
{
  _fromCommandLine = fromCommandLine;
}

void View::snapshot()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
  QImage snapshot = this->grabFramebuffer();
#else
  QImage snapshot = this->grabFrameBuffer(false);
#endif
  QString filename = QString::fromStdString(_title);
  std::stringstream sstr;
  switch (_imageSuffixMode)
  {
    case convert :
      sstr.fill('0');
      sstr << "_" << std::setw(5) << _suffix++;
      break;
    case animate :
      sstr << "_" << _suffix++;
      break;
  }
  filename += QString::fromStdString(sstr.str());
  std::string format;
  switch (_image.getFormat())
  {
    case ImageSaver::png :
      format = "PNG";
      filename += ".png";
      break;
    case ImageSaver::jpeg :
      format = "JPG";
      filename += ".jpg";
      break;
    case ImageSaver::ppm :
      format = "PPM";
      filename += ".ppm";
  }
  if (snapshot.save(filename,format.c_str(),_image.getQuality()))
    std::clog << "Snapshot saved to " << filename.toStdString() << std::endl;
}
