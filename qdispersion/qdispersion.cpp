#include "qdispersion.h"
#include "ui_qdispersion.h"
#include "plot/graph.hpp"
#include <QDebug>

QDispersion::QDispersion(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QDispersion),
  _intValidator(1,10000,this),
  _eigparser(nullptr)
{
  ui->setupUi(this);
  ui->ndiv1->setValidator(&_intValidator);
  QSize size = ui->ndivLabel->sizeHint();
  size.setWidth(size.width()-3);
  ui->labelLabel->setMinimumSize(size);
  this->setSpacerSize();
  ui->leftframe->setDisabled(true);
  connect(ui->plot,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(coordStatusBar(QMouseEvent*)));
}

QDispersion::~QDispersion()
{
    delete ui;
}

void QDispersion::dragEnterEvent(QDragEnterEvent *dragEnterEvent)
{
  if (dragEnterEvent->mimeData()->hasFormat("text/uri-list"))
      dragEnterEvent->acceptProposedAction();
}

void QDispersion::dropEvent(QDropEvent *dropEvent)
{
  QList<QUrl> urls = dropEvent->mimeData()->urls();
  if (urls.isEmpty())
    return;

  QString fileName = urls.first().toLocalFile();
  if (fileName.isEmpty())
    return;
  this->openFile(fileName);
}

void QDispersion::openFile(const QString &filename)
{
  try {
    ui->statusBar->showMessage(tr("Loading file ")+filename);
    ui->leftframe->setDisabled(true);
    _eigparser.reset(EigParser::getEigParser(filename.toStdString()));
    size_t pos = filename.toStdString().find_last_of("/\\");
    ui->filename->setText(QString::fromStdString(filename.toStdString().substr(pos+1)));
    ui->leftframe->setDisabled(false);
    ui->statusBar->showMessage(tr("Ready"));
  }
  catch ( Exception &e ) {
    QMessageBox::critical(this,tr("Error"),QString::fromStdString(e.fullWhat()));
  }
}

void QDispersion::plot() const
{
    QString command;
}

void QDispersion::coordStatusBar(QMouseEvent *event)
{
  double x = ui->plot->xAxis->pixelToCoord(event->pos().x());
  double y = ui->plot->yAxis->pixelToCoord(event->pos().y());
  QString info = "kpt="+QString::number(x) + " E=" + QString::number(y);
  ui->statusBar->showMessage(info);
}

void QDispersion::on_nsegments_valueChanged(int arg1)
{
 auto  children1 = ui->labelWidget->children();
 auto  children2 = ui->ndivWidget->children();
 QLayout* layout1 = ui->labelWidget->layout();
 QLayout* layout2 = ui->ndivWidget->layout();

 int currentSegment = children2.size()-2;
 if (currentSegment < arg1)
   {
     QLayoutItem* spacer1 = layout1->takeAt(layout1->count()-1);
     QLayoutItem* spacer2 = layout2->takeAt(layout2->count()-1);
     for (int n = currentSegment+1; n <= arg1; ++n )
       {
         QLineEdit* line1 = new QLineEdit(ui->labelWidget);
         QLineEdit* line2 = new QLineEdit(ui->ndivWidget);
         line2->setValidator(&_intValidator);
         line1->setObjectName("label"+QString::number(n+1));
         line1->setMaximumWidth(24);
         line2->setObjectName("ndiv"+QString::number(n));
         line2->setMaximumWidth(24);
         ui->labelWidget->layout()->addWidget(line1);
         ui->ndivWidget->layout()->addWidget(line2);
       }
     layout1->addItem(spacer1);
     layout2->addItem(spacer2);
   }
 else
   {
     QObjectList toRemove;
     for (auto c = children1.begin(); c != children1.end(); ++c)
       {
         QString name = (*c)->objectName();
         int id = name.remove(0,5).toInt();
         if (id > arg1+1)
           {
             toRemove.push_back((*c));
           }
       }
     for (auto r = toRemove.begin(); r != toRemove.end() ; ++r)
       delete (*r);
     toRemove.clear();
     for (auto c = children2.begin(); c != children2.end(); ++c)
       {
         QString name = (*c)->objectName();
         int id = name.remove(0,4).toInt();
         if (id > arg1)
           {
             toRemove.push_back((*c));
           }
       }
     for (auto r = toRemove.begin(); r != toRemove.end() ; ++r)
       delete (*r);
   }
  this->setSpacerSize();
}

void QDispersion::setSpacerSize()
{
  int newWidth = ui->label1->size().width();
  QSize actualSize = ui->ndivRight->sizeHint();
  actualSize.setWidth(newWidth/2);
  ui->ndivLeft->changeSize(actualSize.width(),actualSize.height(),QSizePolicy::Fixed);
  ui->ndivRight->changeSize(actualSize.width(),actualSize.height(),QSizePolicy::MinimumExpanding);
}
