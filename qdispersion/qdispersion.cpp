#include "qdispersion.h"
#include "ui_qdispersion.h"
#include "plot/graph.hpp"
#include "plot/gnuplot.hpp"
#include "io/eigparserelectrons.hpp"
#include <QDebug>
#include "dialogs/mendeleev.h"

QDispersion::QDispersion(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QDispersion),
  _magneticMomentum(),
  _intValidator(1,10000,this),
  //_atomValidator(QRegExp("^((((\\d+)-(\\*|(\\d+ )))|((\\d+)( \\d+)+))+)$"),this),
  _atomValidator(QRegExp("^((\\d+)( \\d+)+)$"),this),
  _eigparser(nullptr),
  _currentDirectory()
{
  ui->setupUi(this);
  ui->ndiv1->setValidator(&_intValidator);
  QSize size = ui->ndivLabel->sizeHint();
  size.setWidth(size.width()-3);
  ui->labelLabel->setMinimumSize(size);
  this->setSpacerSize();
  ui->leftframe->setDisabled(true);
  connect(ui->plot,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(coordStatusBar(QMouseEvent*)));
  ui->buttonBox->button(QDialogButtonBox::Save)->setText(tr("&Save"));
  ui->buttonBox->button(QDialogButtonBox::Apply)->setText(tr("&Apply"));
  ui->buttonBox->button(QDialogButtonBox::Close)->setText(tr("&Close"));
  ui->buttonBox->button(QDialogButtonBox::Open)->setText(tr("&Open"));
  ui->atoms->setValidator(&_atomValidator);
  ui->m4->hide();
  ui->m3->hide();
  ui->m2->hide();
  ui->m1->hide();
  ui->p1->hide();
  ui->p2->hide();
  ui->p3->hide();
  ui->p4->hide();
  ui->angular->setCurrentIndex(5);  auto sizes = ui->splitter->sizes();
  sizes[0] = ui->leftframe->sizeHint().width();
  sizes[1] = 10000;
  ui->splitter->setSizes(sizes);
  //ui->splitter->refresh();
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
  this->setCursor(Qt::WaitCursor);
  try {
    ui->statusBar->showMessage(tr("Loading file ")+filename);
    ui->leftframe->setDisabled(true);
    _eigparser.reset(EigParser::getEigParser(filename.toStdString()));
    size_t pos = filename.toStdString().find_last_of("/\\");
    ui->filename->setText(QString::fromStdString(filename.toStdString().substr(pos+1)));
    ui->leftframe->setDisabled(false);
    ui->statusBar->showMessage(tr("Ready"));
    auto ndiv = _eigparser->getNdiv();
    auto labels = _eigparser->getLabels();
    ui->nsegments->setValue(ndiv.size());
    if ( ndiv.size()!=0 && ndiv.size()==labels.size()-1)
      {
        auto  children1 = ui->labelWidget->children();
        auto  children2 = ui->ndivWidget->children();
        for (auto c = children1.begin(); c != children1.end(); ++c)
          {
            QString name = (*c)->objectName();
            int id = name.remove(0,5).toInt();
            if (id>0)
              {
                reinterpret_cast<QLineEdit*>(*c)->setText(QString::fromStdString(labels[id-1]));
              }
          }
        for (auto c = children2.begin(); c != children2.end(); ++c)
          {
            QString name = (*c)->objectName();
            int id = name.remove(0,4).toInt();
            if (id>0)
              {
                reinterpret_cast<QLineEdit*>(*c)->setText(QString::number(ndiv[id-1]));
              }
          }
      }
    else
      {
        ui->ndiv1->setText(QString::number(_eigparser->getPath().size()));
      }
    ui->ignore->setMaximum(_eigparser->getNband());
    bool electronic = (dynamic_cast<EigParserElectrons*>(_eigparser.get())!=nullptr);
    ui->ignoreWidget->setEnabled(electronic);
    ui->ignore->setEnabled(electronic);
    ui->ignore->setValue(0);
    try
    {
      _eigparser->getBandColor(1,1);
      ui->fatbands->setEnabled(true);
      if (ui->fatbands->isChecked())
        {
          bool electronic = (dynamic_cast<EigParserElectrons*>(_eigparser.get())!=nullptr);
          ui->angularLabel->setEnabled(electronic);
          ui->angular->setEnabled(electronic);
          ui->magneticLabel->setEnabled(electronic);
          ui->magneticSerie->setEnabled(electronic);
        }
    }
    catch (...)
    {
      ui->fatbands->setEnabled(false);
      ui->fatbands->setChecked(false);
    }
    pos = filename.lastIndexOf(QRegExp("[/\\\\]"));
    _currentDirectory = filename.left(pos+1);
    QDispersion::plot();
  }
  catch ( Exception &e ) {
    QMessageBox::critical(this,tr("Error"),QString::fromStdString(e.fullWhat()));
  }
  this->setCursor(Qt::ArrowCursor);
}

void QDispersion::plot(bool commandOnly)
{
  QString command(":plot ");
  Graph::GraphSave save = Graph::NONE;
  if (ui->saveCommand->isChecked()) {command = ":save "; save = Graph::PRINT;}
  else if (ui->dataCommand->isChecked()) {command = ":data "; save = Graph::DATA;}
  command += "band ";
  command += QString::fromStdString(_eigparser->getFilename())+" ";

  // Unit
  UnitConverter eunit = UnitConverter::getFromString(ui->energyUnit->currentData().toString().toStdString());
  command += QString("eunit ")+QString::fromStdString(eunit.str());

  int hasNdiv = 0;
  //ndiv
  auto  children2 = ui->ndivWidget->children();
  QString commandTmp = " ndiv ";
  for (auto c = children2.begin(); c != children2.end(); ++c)
    {
      QLineEdit *child = reinterpret_cast<QLineEdit*>(*c);
      QString name = child->objectName();
      int id = name.remove(0,4).toInt();
      if (id>0)
        {
          QString text = child->text();
          if (!text.isEmpty()) hasNdiv++;
          commandTmp +=text+":";
        }
    }
  if (commandTmp[commandTmp.size()-1]==':')
    commandTmp.chop(1);
  if (hasNdiv == ui->nsegments->value()) command+=commandTmp;
  commandTmp.clear();

  //labels
  auto  children1 = ui->labelWidget->children();
  commandTmp += " labels ";
  int hasNlabels = 0;
  for (auto c = children1.begin(); c != children1.end(); ++c)
    {
      QLineEdit *child = reinterpret_cast<QLineEdit*>(*c);
      QString name = child->objectName();
      int id = name.remove(0,5).toInt();
      if (id>0)
        {
          QString text = child->text();
          if (!text.isEmpty()) hasNlabels++;
          commandTmp += text+":";
        }
    }
  if (commandTmp[commandTmp.size()-1]==':')
    commandTmp.chop(1);
  if (hasNlabels==hasNdiv+1 && hasNdiv>0) command+=commandTmp;

  // fermi
  if (std::abs(ui->fermi->value())>1e-7)
  {
    command += " fermi ";
    UnitConverter fermiUnit = UnitConverter::getFromString(ui->fermiUnit->currentData().toString().toStdString());
    fermiUnit = UnitConverter::Ha;
    command += QString::number(ui->fermi->value()*fermiUnit,'g',14);
  }

  // ignore
  if (ui->ignore->isEnabled() && ui->ignore->value() > 0) command += " ignore "+QString::number(ui->ignore->value());

  // fatbands
  if (ui->fatbands->isChecked())
    {
      command += " fatband ";
      if (!ui->atoms->text().isEmpty())
        {
          command += ui->atoms->text();
        }
      if (ui->angular->isEnabled() && ui->angular->currentIndex() != 5)
        {
          command += " angular "+QString::number(ui->angular->currentIndex());
          QString commandMagnetic;
          if (ui->m4->isChecked()) commandMagnetic += " -4";
          if (ui->m3->isChecked()) commandMagnetic += " -3";
          if (ui->m2->isChecked()) commandMagnetic += " -2";
          if (ui->m1->isChecked()) commandMagnetic += " -1";
          if (ui->z ->isChecked()) commandMagnetic += "  0";
          if (ui->p1->isChecked()) commandMagnetic += " +1";
          if (ui->p2->isChecked()) commandMagnetic += " +2";
          if (ui->p3->isChecked()) commandMagnetic += " +3";
          if (ui->p4->isChecked()) commandMagnetic += " +4";
          if (!commandMagnetic.isEmpty()) command += " magnetic "+commandMagnetic;
        }
    }

  ui->command->setText(command);
  if (!commandOnly)
  {
    ConfigParser parser;
    parser.setSensitive(true);
    parser.setContent(command.toStdString());
    try
    {
      ui->statusBar->showMessage(tr("Plot in progress"));
      this->setCursor(Qt::WaitCursor);
      Graph::plotBand(*_eigparser.get(),parser,ui->plot,save);
      if (save == Graph::PRINT)
      {
        Gnuplot gplt;
        Graph::plotBand(*_eigparser.get(),parser,&gplt,save);
      }
      ui->statusBar->clearMessage();
    }
    catch (Exception &e)
    {
      ui->statusBar->showMessage(QString::fromStdString(e.what()));
      QMessageBox::critical(this,tr("Error"),QString::fromStdString(e.fullWhat()));
    }
    this->setCursor(Qt::ArrowCursor);
  }
}

void QDispersion::coordStatusBar(QMouseEvent *event)
{
  double x = ui->plot->xAxis->pixelToCoord(event->pos().x());
  double y = ui->plot->yAxis->pixelToCoord(event->pos().y());
  QString info = "kpt="+QString::number(x) + " E=" + QString::number(y) + QString::fromStdString(ui->energyUnit->currentUnit().str());
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
          line1->setMaximumWidth(32);
          line1->setAlignment(Qt::AlignHCenter);
          line2->setObjectName("ndiv"+QString::number(n));
          line2->setMaximumWidth(32);
          line2->setAlignment(Qt::AlignHCenter);
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

void QDispersion::on_fermi_editingFinished()
{
  QDispersion::plot();
}

void QDispersion::on_fermiUnit_currentIndexChanged(int index)
{
  (void) index;
  QDispersion::plot();
}

void QDispersion::on_energyUnit_currentIndexChanged(int index)
{
  (void) index;
  QDispersion::plot();
}

void QDispersion::on_ignore_valueChanged(int arg1)
{
  (void) arg1;
  QDispersion::plot();
}

void QDispersion::on_buttonBox_clicked(QAbstractButton *button)
{
  QDialogButtonBox::StandardButton b = ui->buttonBox->standardButton(button);
  if ( b == QDialogButtonBox::Save )
    {
      QString filename = QFileDialog::getSaveFileName(this,tr("PDF file"),_currentDirectory,"PDF (*.pdf)",nullptr,QFileDialog::DontUseNativeDialog);
      if (!filename.endsWith(".pdf")) filename += ".pdf";
      ui->plot->savePdf(filename,0,0,QCP::epNoCosmetic);
      int pos = filename.lastIndexOf(QRegExp("[/\\\\]"));
      _currentDirectory = filename.left(pos+1);
    }
  else if ( b == QDialogButtonBox::Open )
    {
      QString filename = QFileDialog::getOpenFileName(this,tr("Dispersion file"),"","",nullptr,QFileDialog::DontUseNativeDialog);
      if ( !filename.isEmpty() )
        this->openFile(filename);
    }
  else if  (b ==QDialogButtonBox::Apply )
    {
      QDispersion::plot();
    }
  else if ( b == QDialogButtonBox::Close )
    this->close();
}

void QDispersion::on_angular_currentIndexChanged(int index)
{
  switch (index)
    {
    case 0:
  ui->m4->hide(); ui->m4->setChecked(false);
  ui->m3->hide(); ui->m3->setChecked(false);
  ui->m2->hide(); ui->m2->setChecked(false);
  ui->m1->hide(); ui->m1->setChecked(false);
  ui->z->show() ; ui->z ->setChecked(false);
  ui->p1->hide(); ui->p1->setChecked(false);
  ui->p2->hide(); ui->p2->setChecked(false);
  ui->p3->hide(); ui->p3->setChecked(false);
  ui->p4->hide(); ui->p4->setChecked(false);
      break;
    case 1:
  ui->m4->hide(); ui->m4->setChecked(false);
  ui->m3->hide(); ui->m3->setChecked(false);
  ui->m2->hide(); ui->m2->setChecked(false);
  ui->m1->show(); ui->m1->setChecked(false);
  ui->z->show() ; ui->z ->setChecked(false);
  ui->p1->show(); ui->p1->setChecked(false);
  ui->p2->hide(); ui->p2->setChecked(false);
  ui->p3->hide(); ui->p3->setChecked(false);
  ui->p4->hide(); ui->p4->setChecked(false);
      break;
    case 2:
  ui->m4->hide(); ui->m4->setChecked(false);
  ui->m3->hide(); ui->m3->setChecked(false);
  ui->m2->show(); ui->m2->setChecked(false);
  ui->m1->show(); ui->m1->setChecked(false);
  ui->z->show() ; ui->z ->setChecked(false);
  ui->p1->show(); ui->p1->setChecked(false);
  ui->p2->show(); ui->p2->setChecked(false);
  ui->p3->hide(); ui->p3->setChecked(false);
  ui->p4->hide(); ui->p4->setChecked(false);
      break;
    case 3:
  ui->m4->hide(); ui->m4->setChecked(false);
  ui->m3->show(); ui->m3->setChecked(false);
  ui->m2->show(); ui->m2->setChecked(false);
  ui->m1->show(); ui->m1->setChecked(false);
  ui->z->show() ; ui->z ->setChecked(false);
  ui->p1->show(); ui->p1->setChecked(false);
  ui->p2->show(); ui->p2->setChecked(false);
  ui->p3->show(); ui->p3->setChecked(false);
  ui->p4->hide(); ui->p4->setChecked(false);
      break;
    case 4:
  ui->m4->show(); ui->m4->setChecked(false);
  ui->m3->show(); ui->m3->setChecked(false);
  ui->m2->show(); ui->m2->setChecked(false);
  ui->m1->show(); ui->m1->setChecked(false);
  ui->z->show() ; ui->z ->setChecked(false);
  ui->p1->show(); ui->p1->setChecked(false);
  ui->p2->show(); ui->p2->setChecked(false);
  ui->p3->show(); ui->p3->setChecked(false);
  ui->p4->show(); ui->p4->setChecked(false);
      break;
    case 5:
  ui->m4->hide(); ui->m4->setChecked(false);
  ui->m3->hide(); ui->m3->setChecked(false);
  ui->m2->hide(); ui->m2->setChecked(false);
  ui->m1->hide(); ui->m1->setChecked(false);
  ui->z->hide() ; ui->z ->setChecked(false);
  ui->p1->hide(); ui->p1->setChecked(false);
  ui->p2->hide(); ui->p2->setChecked(false);
  ui->p3->hide(); ui->p3->setChecked(false);
  ui->p4->hide(); ui->p4->setChecked(false);
      break;
    }
}

void QDispersion::on_fatbands_clicked(bool checked)
{
   if (checked)
     {
       bool electronic = (dynamic_cast<EigParserElectrons*>(_eigparser.get())!=nullptr);
       ui->angularLabel->setEnabled(electronic);
       ui->angular->setEnabled(electronic);
       ui->magneticLabel->setEnabled(electronic);
       ui->magneticSerie->setEnabled(electronic);
       if (!electronic) QDispersion::plot();
     }
}

void QDispersion::on_fermi_valueChanged(double arg1)
{
  (void) arg1;
  QDispersion::plot();
}

void QDispersion::on_mendeleev_clicked()
{
   Mendeleev m(this);
   m.build();
   m.exec();
   if (m.result()!=0) QDispersion::plot();
}

void QDispersion::on_plotCommand_clicked(bool checked)
{
  this->plot(checked);
}

void QDispersion::on_saveCommand_clicked(bool checked)
{
  this->plot(checked);
}

void QDispersion::on_dataCommand_clicked(bool checked)
{
  this->plot(checked);
}
