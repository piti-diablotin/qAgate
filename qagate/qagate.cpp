#include "qagate.h"
#include "ui_qagate.h"
#include <QMetaObject>
#include <QMetaMethod>
#include <QDebug>
#include <QMap>
#include "tabs/abstracttab.h"
#include <QMessageBox>
#include "canvas/canvaslocal.hpp"
#include "canvas/canvasphonons.hpp"
#include "canvas/canvasdensity.hpp"

qAgate::qAgate(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::qAgate),
  _tabHiden(false),
  _homeNeedsDistance(false),
  _homeNeedsAngle(false),
  _canvasButtons(new QButtonGroup(this))
{
  ui->setupUi(this);
  for (int index = 0; index < ui->tabWidget->count(); ++index)
  {
    AbstractTab* tab = dynamic_cast<AbstractTab*>(ui->tabWidget->widget(index));
    tab->plugActions(this);
    connect(tab,SIGNAL(sendCommand(QString,bool)),ui->view,SLOT(processCommand(QString,bool)));
    connect(tab,SIGNAL(plotChanged(QPlot*)),this,SLOT(setPlot(QPlot*)));
  }

  int indexPhonons = ui->tabWidget->indexOf(ui->phonons);
  int indexLocal = ui->tabWidget->indexOf(ui->local);
  int indexDensite = ui->tabWidget->indexOf(ui->density);
  int indexPositions = ui->tabWidget->indexOf(ui->md);
  QRadioButton* boxPhonons = new QRadioButton(this); boxPhonons->setObjectName("mode phonons");
  QRadioButton* boxLocal = new QRadioButton(this); boxLocal->setObjectName("mode local");
  QRadioButton* boxDensite = new QRadioButton(this); boxDensite->setObjectName("mode density");
  QRadioButton* boxPositions = new QRadioButton(this); boxPositions->setObjectName("mode positions");
  boxPositions->setChecked(true);
  QTabBar* tabBar = ui->tabWidget->tabBar();
  tabBar->setTabButton(indexPhonons,QTabBar::RightSide,boxPhonons);
  tabBar->setTabButton(indexLocal,QTabBar::RightSide,boxLocal);
  tabBar->setTabButton(indexDensite,QTabBar::RightSide,boxDensite);
  tabBar->setTabButton(indexPositions,QTabBar::RightSide,boxPositions);
  _canvasButtons->addButton(boxPhonons,0);
  _canvasButtons->addButton(boxLocal,1);
  _canvasButtons->addButton(boxDensite,2);
  _canvasButtons->addButton(boxPositions,3);
  connect(_canvasButtons,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(updateCanvas(QAbstractButton*)));

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
  connect(ui->settings,SIGNAL(switchFilling()),this,SLOT(manageSignal()));
  connect(ui->settings,SIGNAL(switchLight()),this,SLOT(manageSignal()));
  connect(ui->settings,SIGNAL(switchPerspective()),this,SLOT(manageSignal()));
  connect(ui->settings,SIGNAL(switchAA()),this,SLOT(manageSignal()));

  // visuals
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

  // home
  connect(ui->home,SIGNAL(needAngle(bool)),this,SLOT(setNeeds(bool)));
  connect(ui->home,SIGNAL(needDistance(bool)),this,SLOT(setNeeds(bool)));

  // Phonons
  connect(ui->phonons,SIGNAL(needPartialUpdate(void)),this,SLOT(setNeeds()));
  connect(ui->phonons,SIGNAL(needFullUpdate(void)),this,SLOT(setNeeds()));

  // Self
  connect(this,SIGNAL(emitCommand(QString,bool)),ui->view,SLOT(processCommand(QString,bool)));

  this->updateTab();
  this->setPlot(AbstractTab::plot());

}

qAgate::~qAgate()
{
  ui->view->canvas()->setGraph(nullptr);
  delete ui;
}

void qAgate::initInput(int argc, const char **argv)
{
  std::vector<std::string> filename;
  for ( int i = 0 ; i < argc ; ++i )
  {
    std::ifstream file(argv[i],std::ios::in);

    if ( file.good() && (i == 0 || (i > 0 && strcmp(argv[i-1],"-c") != 0 && strcmp(argv[i-1],"--config")) != 0) )
    {
      filename.push_back(std::string(argv[i]));
    }

    file.close();
  }
  try
  {
    if ( filename.size() > 0 && ui->view != nullptr)
    {
      try
      {
        emit(emitCommand(QString::fromStdString(":open "+filename[0]),false));
      }
      catch ( Exception &e ) {
        if ( e.getReturnValue() == ERRDIV || e.getReturnValue() == ERRABT  )
          throw e;
      }

      for ( unsigned file = 1; file < filename.size() ; ++file )
      {
        try
        {
          emit(emitCommand(QString::fromStdString(":append "+filename[file]),false)); // Do not pop inputChar
        }
        catch ( Exception &e )
        {
          e.ADD(tr("Ignoring file ").toStdString()+filename[file], ERRWAR);
          QMessageBox::critical(this,tr("Error"),QString::fromStdString(e.fullWhat()));
        }
      }
    }
  }
  catch (Exception& e)
  {
    e.ADD(tr("Updating canvas failed").toStdString(),ERRDIV);
    QMessageBox::critical(this,tr("Error"),QString::fromStdString(e.fullWhat()));
  }
}

void qAgate::setParameters(const string &config)
{
  ui->view->setParameters(config);
}

void qAgate::updateNeeds()
{
  if (ui->tabWidget->currentWidget() == ui->home )
  {
    if (_homeNeedsAngle)
    {
      int atom1, atom2, atom3;
      ui->home->getAngleAtoms(atom1,atom2,atom3);
      ui->home->setAngle(ui->view->getAngle(atom1,atom2,atom3));
    }
    if (_homeNeedsDistance)
    {
      int atom1, atom2;
      ui->home->getDistanceAtoms(atom1,atom2);
      ui->home->setDistance(ui->view->getDistance(atom1,atom2));
    }
  }
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
    int max = ui->timeLine->timeTotal();
    if (max!=canvas->ntime())
    {
      int ntime = canvas->ntime();
      ui->mediaPlayer->setDisabledMovie(ntime<2);
      ui->timeLine->setTimes(canvas->tbegin(),std::max(canvas->tend(),0),canvas->itime(),ntime);
    }
    else {
      ui->timeLine->setTime(canvas->itime());
      this->updateNeeds();
    }
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
    //ui->mediaPlayer->setDisabledAppend(false);
    this->setWindowTitle(filename+" - qAgate");
    ui->timeLine->setEnabled(true);
    this->updateTab();
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
    _canvasButtons->blockSignals(true);
    if (dynamic_cast<CanvasPhonons*>(canvas)) _canvasButtons->button(0)->setChecked(true);
    else if (dynamic_cast<CanvasLocal*>(canvas)) _canvasButtons->button(1)->setChecked(true);
    else if (dynamic_cast<CanvasDensity*>(canvas)) _canvasButtons->button(2)->setChecked(true);
    else if (dynamic_cast<CanvasPos*>(canvas)) _canvasButtons->button(3)->setChecked(true);
    _canvasButtons->blockSignals(false);
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

void qAgate::setNeeds(bool need)
{
  QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());
  auto signal = metaMethod.name();
  if (signal=="needAngle") _homeNeedsAngle = need;
  else if (signal=="needDistance") _homeNeedsDistance = need;
  else if (signal=="needPartialUpdate" && sender() == ui->phonons ) {
    ui->phonons->partialUpdate(dynamic_cast<CanvasPhonons*>(ui->view->canvas()));
    return;
  }
  else if (signal=="needFullUpdate" && sender() == ui->phonons ) {
    ui->view->setFromCommandLine(true);
    return;
  }
  this->updateNeeds();
}

void qAgate::setPlot(QPlot *plot)
{
  ui->view->canvas()->setGraph(plot);
}

void qAgate::updateCanvas(QAbstractButton *button)
{
  ui->view->setFromCommandLine(true);
  emit(emitCommand(":"+button->objectName()));
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
  ui->tabWidget->widget(index)->show();
  /*
  if (ui->tabWidget->currentWidget() == ui->local)
  {
    ui->view->setFromCommandLine(true);
    emit(emitCommand(":mode local"));
  }
  else if (ui->tabWidget->currentWidget() == ui->density)
  {
    ui->view->setFromCommandLine(true);
    emit(emitCommand(":mode density"));
  }
  else if (ui->tabWidget->currentWidget() == ui->phonons)
  {
    ui->view->setFromCommandLine(true);
    qDebug() << "node phonons";
    emit(emitCommand(":mode phonons"));
  }
  else {
    if(dynamic_cast<CanvasLocal*>(ui->view->canvas())
       ||dynamic_cast<CanvasPhonons*>(ui->view->canvas())
       ||dynamic_cast<CanvasDensity*>(ui->view->canvas()))
      emit(emitCommand(":mode positions"));
  }
  */
  this->updateTab();
}
