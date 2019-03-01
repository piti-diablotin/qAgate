#include "qagate.h"
#include "ui_qagate.h"
#include <QMetaObject>
#include <QMetaMethod>
#include <QDebug>
#include <QMap>
#include "abstracttab.h"

qAgate::qAgate(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::qAgate),
  _tabHiden(false)
{
  ui->setupUi(this);
  for (int index = 0; index < ui->tabWidget->count(); ++index)
    dynamic_cast<AbstractTab*>(ui->tabWidget->widget(index))->plugActions(this);

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
  connect(ui->view,SIGNAL(mouseInput()),this,SLOT(syncWithUserInput()));
  connect(ui->view,SIGNAL(newSize(int,int)),ui->settings,SLOT(updateDisplaySize(int,int)));

  // logger
  connect(ui->logger,SIGNAL(debugMode(bool)),ui->view,SLOT(setDebugMode(bool)));
  connect(ui->logger,SIGNAL(copied(QString,int)),ui->statusBar,SLOT(showMessage(QString,int)));

  // settings
  connect(ui->settings,SIGNAL(sendCommand(QString,bool)),ui->view,SLOT(processCommand(QString,bool)));
  connect(ui->settings,SIGNAL(switchFilling()),this,SLOT(manageSignal()));
  connect(ui->settings,SIGNAL(switchLight()),this,SLOT(manageSignal()));
  connect(ui->settings,SIGNAL(switchPerspective()),this,SLOT(manageSignal()));
  connect(ui->settings,SIGNAL(switchAA()),this,SLOT(manageSignal()));

  // visuals
  connect(ui->visuals,SIGNAL(sendCommand(QString,bool)),ui->view,SLOT(processCommand(QString,bool)));
  connect(ui->visuals,SIGNAL(switchTimeInfo()),this,SLOT(manageSignal()));
  connect(ui->visuals,SIGNAL(switchAngles()),this,SLOT(manageSignal()));
  connect(ui->visuals,SIGNAL(translatePX()),this,SLOT(manageSignal()));
  connect(ui->visuals,SIGNAL(translateMX()),this,SLOT(manageSignal()));
  connect(ui->visuals,SIGNAL(translatePY()),this,SLOT(manageSignal()));
  connect(ui->visuals,SIGNAL(translateMY()),this,SLOT(manageSignal()));
  connect(ui->visuals,SIGNAL(translatePZ()),this,SLOT(manageSignal()));
  connect(ui->visuals,SIGNAL(translateMZ()),this,SLOT(manageSignal()));
  connect(ui->visuals,SIGNAL(alongX()),this,SLOT(manageSignal()));
  connect(ui->visuals,SIGNAL(alongY()),this,SLOT(manageSignal()));
  connect(ui->visuals,SIGNAL(alongZ()),this,SLOT(manageSignal()));

  // Self
  connect(this,SIGNAL(emitCommand(QString)),ui->view,SLOT(processCommand(QString)));
  this->updateTab();
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
    {"timeChanged", ":"},
    {"switchFilling", "o"},
    {"switchLight","l"},
    {"switchPerspective","p"},
    {"switchAA","A"},
    {"switchAngles","a"},
    {"switchTimeInfo","t"},
    {"translatePX","+x"},
    {"translateMX","-x"},
    {"translatePY","+y"},
    {"translateMY","-y"},
    {"translatePZ","+z"},
    {"translateMZ","-z"},
    {"alongX","x"},
    {"alongY","y"},
    {"alongZ","z"},
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
  else if (signal=="fileOpened")
    {
      ui->mediaPlayer->setDisabledAppend(false);
      this->setWindowTitle(filename+" - qAgate");
      ui->timeLine->setEnabled(true);
    }
  else if (signal == "sendCommand")
    {
      emit(emitCommand(filename));
    }

}

void qAgate::syncWithUserInput()
{
  QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());
  auto signal = metaMethod.name();
  if ( signal == "userInput")
    {
      this->updateTab();
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
  else if (signal == "mouseInput")
    {
      ui->visuals->updateAngles(ui->view);
    }
}

void qAgate::updateTab()
{
  dynamic_cast<AbstractTab*>(ui->tabWidget->currentWidget())->update(ui->view);
  if (ui->tabWidget->currentWidget() == ui->visuals)
    ui->visuals->updateAngles(ui->view);
}

void qAgate::on_tabWidget_tabBarClicked(int index)
{
   if (index == ui->tabWidget->currentIndex())
     {
       _tabHiden ?
             ui->tabWidget->currentWidget()->show()
           : ui->tabWidget->currentWidget()->hide();
       _tabHiden = !_tabHiden;
       if (_tabHiden)
         {
           for (int i = 0; i < ui->tabWidget->count(); ++i)
             ui->tabWidget->widget(i)->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
         }
       else
         {
           for (int i = 0; i < ui->tabWidget->count(); ++i)
             ui->tabWidget->widget(i)->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
         }
     }
}

void qAgate::on_tabWidget_currentChanged(int index)
{
  if (_tabHiden)
    {
      for (int i = 0; i < ui->tabWidget->count(); ++i)
        ui->tabWidget->widget(i)->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
      _tabHiden = !_tabHiden;
    }
  ui->tabWidget->currentWidget()->show();
  this->updateTab();
}
