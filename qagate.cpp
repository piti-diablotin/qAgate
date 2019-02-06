#include "qagate.h"
#include "ui_qagate.h"
#include <QMetaObject>
#include <QMetaMethod>
#include <QDebug>
#include <QMap>

qAgate::qAgate(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::qAgate)
{
  ui->setupUi(this);
  // MediaPlayer
  connect(ui->mediaPlayer,SIGNAL(play()),this,SLOT(manageSignal()));
  connect(ui->mediaPlayer,SIGNAL(pause()),this,SLOT(manageSignal()));
  connect(ui->mediaPlayer,SIGNAL(norepeat()),this,SLOT(manageSignal()));
  connect(ui->mediaPlayer,SIGNAL(repeat()),this,SLOT(manageSignal()));
  connect(ui->mediaPlayer,SIGNAL(palindrome()),this,SLOT(manageSignal()));
  connect(ui->mediaPlayer,SIGNAL(next()),this,SLOT(manageSignal()));
  connect(ui->mediaPlayer,SIGNAL(previous()),this,SLOT(manageSignal()));
  connect(ui->mediaPlayer,SIGNAL(faster()),this,SLOT(manageSignal()));
  connect(ui->mediaPlayer,SIGNAL(slower()),this,SLOT(manageSignal()));
  connect(ui->mediaPlayer,SIGNAL(snapshot()),this,SLOT(manageSignal()));
  connect(ui->mediaPlayer,SIGNAL(record()),this,SLOT(manageSignal()));
  connect(ui->mediaPlayer,SIGNAL(stopRecord()),this,SLOT(manageSignal()));
  connect(ui->mediaPlayer,SIGNAL(open(QString)),this,SLOT(manageSignal(QString)));
  connect(ui->mediaPlayer,SIGNAL(append(QString)),this,SLOT(manageSignal(QString)));
  connect(ui->mediaPlayer,SIGNAL(zoomIn()),ui->view,SLOT(zoomIn()));
  connect(ui->mediaPlayer,SIGNAL(zoomOut()),ui->view,SLOT(zoomOut()));

  // timeLine
  connect(ui->timeLine,SIGNAL(timeBeginChanged()),this,SLOT(manageSignal()));
  connect(ui->timeLine,SIGNAL(timeEndChanged()),this,SLOT(manageSignal()));
  connect(ui->timeLine,SIGNAL(timeChanged()),this,SLOT(manageSignal()));

  // view
  connect(ui->view,SIGNAL(fileOpened(QString)),this,SLOT(manageSignal(QString)));
  connect(ui->view,SIGNAL(updated()),this,SLOT(manageSignal()));
  connect(ui->view,SIGNAL(closeMe()),this,SLOT(close()));
  connect(ui->view,SIGNAL(userInput()),this,SLOT(syncWithUserInput()));

  // Self
  connect(this,SIGNAL(emitCommand(QString)),ui->view,SLOT(processCommand(QString)));
}

qAgate::~qAgate()
{
  delete ui;
}

void qAgate::manageSignal()
{
  QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());
  auto signal = metaMethod.name();
  QString command;
  static const QMap<QString,QString> mapping{
    {"play", " "},
    {"pause", " "},
    {"norepeat", ":r 1"},
    {"repeat", ":r -1"},
    {"palindrome", ":r -2"},
    {"next", ">"},
    {"previous", "<"},
    {"faster", "*"},
    {"slower", "/"},
    {"snapshot", "s"},
    {"record", "m"},
    {"stopRecord", "m"},
    {"timeBeginChanged", ":tbegin "},
    {"timeEndChanged", ":tend "},
    {"timeChanged", ":"}
  };
  if (mapping.contains(signal))
    {
      command = mapping[signal];
      if ( signal.contains("timeBeginChanged")) command+=QString::number(ui->timeLine->timeBegin());
      else if ( signal.contains("timeEndChanged")) command+=QString::number(ui->timeLine->timeEnd());
      else if ( signal.contains("timeChanged")) command+=QString::number(ui->timeLine->time());
      emit(emitCommand(command));
    }
  if (signal == "updated")
    {
      auto canvas = ui->view->canvas();
      ui->mediaPlayer->setPlay(!canvas->isPaused());
      ui->timeLine->setTimes(canvas->tbegin(),std::max(canvas->tend(),0),canvas->itime(),canvas->ntime());
    }
}

void qAgate::manageSignal(QString filename)
{
  QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());
  auto signal = metaMethod.name();
  QString command;
  static const QMap<QString,QString> mapping{
    {"open", ":open"},
    {"append", ":append"},
  };
  if (mapping.contains(signal))
    {
      command = mapping[signal]+" "+filename;
      emit(emitCommand(command));
    }
  if (signal=="fileOpened")
    {
      ui->mediaPlayer->setDisabledAppend(false);
      this->setWindowTitle(filename+" - qAgate");
      ui->timeLine->setEnabled(true);
    }

}

void qAgate::syncWithUserInput()
{
  if (ui->view->canvas() == nullptr) return;
  auto canvas = ui->view->canvas();
  MediaPlayer::RepeatMode repeat;
  switch ( canvas->nLoop() ) {
    case -2 :
      repeat = MediaPlayer::Palindrome;
      break;
    case -1 :
      repeat = MediaPlayer::Repeat;
      break;
    default :
      repeat = MediaPlayer::None;
    }
  ui->mediaPlayer->setRepeatMode(repeat);

  int ntime = std::max(canvas->ntime(),1);
  ui->mediaPlayer->setDisabledMovie(ntime<2);
  ui->timeLine->setTimes(canvas->tbegin(),std::max(canvas->tend(),0),canvas->itime(),canvas->ntime());
}
