#include "qdos.h"
#include "ui_qdos.h"
#include <QDoubleValidator>
#include "qdos/adddos.h"
#include "tools/coloritemdelegate.h"

QDos::QDos(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QDos),
  _currentFolder(),
  _db(),
  _plotCurves(),
  _energyUnit()
{
  ui->setupUi(this);
  ui->unitDisplay->setText(QString::fromStdString(ui->unit->currentUnit().str()));
  ui->xmin->setValidator(new QDoubleValidator(this));
  ui->xmax->setValidator(new QDoubleValidator(this));
  ui->ymin->setValidator(new QDoubleValidator(this));
  ui->ymax->setValidator(new QDoubleValidator(this));
  ui->add->setDefaultAction(ui->actionAdd);
  ui->actionAdd->setDisabled(true);
  ui->remove->setDefaultAction(ui->actionRemove);
  ui->toDraw->setModel(&_plotCurves);
  ui->toDraw->horizontalHeader()->setStretchLastSection(true);
  ui->toDraw->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  ui->spin->setItemData(1,"1");
  ui->spin->setItemData(2,"2");
  ui->paw->setItemData(0,"ae");
  ui->paw->setItemData(1,"ps");
  ui->paw->setItemData(2,"pw");
  _currentFolder = ui->folder->text();
  connect(ui->plot,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(coordStatusBar(QMouseEvent*)));
  ui->buttonBox->button(QDialogButtonBox::Save)->setText(tr("&Save"));
  ui->buttonBox->button(QDialogButtonBox::Apply)->setText(tr("&Apply"));
  ui->buttonBox->button(QDialogButtonBox::Close)->setText(tr("&Close"));
  ui->buttonBox->button(QDialogButtonBox::Reset)->setText(tr("&Reset"));
  _energyUnit = ui->unit->currentUnit();
  connect(ui->plot->xAxis,SIGNAL(rangeChanged(QCPRange)),this,SLOT(adjustXRange(QCPRange)));
  connect(ui->plot->yAxis,SIGNAL(rangeChanged(QCPRange)),this,SLOT(adjustYRange(QCPRange)));
  ui->toDraw->setItemDelegateForColumn(3,new ColorItemDelegate(this));
}

QDos::~QDos()
{
  delete ui;
}

void QDos::on_unit_currentIndexChanged(const QString &arg1)
{
  ui->unitDisplay->setText(arg1);
  UnitConverter newUnit = ui->unit->currentUnit();
  newUnit.rebase(_energyUnit.base());
  if ( !ui->xmin->text().isEmpty() )
  {
    double xmin = ui->xmin->text().toDouble()/_energyUnit*newUnit;
    ui->xmin->setText(QString::number(xmin));
  }
  if ( !ui->xmax->text().isEmpty() )
  {
    double xmax = ui->xmax->text().toDouble()/_energyUnit*newUnit;
    ui->xmax->setText(QString::number(xmax));
  }
  if ( !ui->ymin->text().isEmpty() )
  {
    double ymin = ui->ymin->text().toDouble()*_energyUnit/newUnit;
    ui->ymin->setText(QString::number(ymin));
  }
  if ( !ui->ymax->text().isEmpty() )
  {
    double ymax = ui->ymax->text().toDouble()*_energyUnit/newUnit;
    ui->ymax->setText(QString::number(ymax));
  }
  _energyUnit = newUnit;
  this->updateAgateCommand();
}

void QDos::on_browse_clicked()
{
  auto dirName = QFileDialog::getExistingDirectory(this,"Chose a directory",_currentFolder,QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
  if ( !dirName.isEmpty() )
  {
    ui->folder->setText(dirName);
    _currentFolder = dirName;
    this->updateAgateCommand();
  }
}

QString QDos::currentFolder() const
{
  return _currentFolder;
}

void QDos::setCurrentFolder(const QString &currentFolder)
{
  _currentFolder = currentFolder;
  ui->folder->setText(_currentFolder);
}

void QDos::coordStatusBar(QMouseEvent *event)
{
  double x = ui->plot->xAxis->pixelToCoord(event->pos().x());
  double y = ui->plot->yAxis->pixelToCoord(event->pos().y());
  QString info = "E="+QString::number(x) + QString::fromStdString(ui->unit->currentUnit().str()) + " DOS=" + QString::number(y);
  ui->statusBar->showMessage(info);
}

void QDos::on_load_clicked()
{
  try
  {
    std::string prefix = ui->folder->text().toStdString()+"/"+ui->prefix->text().toStdString();
    _db.clear();
    _db.buildFromPrefix(prefix);
  }
  catch (Exception &e)
  {
    ui->statusBar->showMessage(QString::fromStdString(e.what()));
    QMessageBox::critical(this,tr("Error"),QString::fromStdString(e.fullWhat()));
    return;
  }
  _plotCurves.clear();
  if (_db.list().size() > 0)
  {
    ui->add->setEnabled(true);
    ui->actionAdd->setEnabled(true);
    ui->paw->setEnabled(_db.atom(_db.list().front()).pawDecomposition());
    ui->spin->setEnabled(_db.atom(_db.list().front()).nsppol()==2);
    ui->updown->setEnabled(ui->spin->currentIndex()==0&&ui->spin->isEnabled());
  }
  _plotCurves.setHeaderData(0,Qt::Horizontal,tr("Selection"));
  _plotCurves.setHeaderData(1,Qt::Horizontal,tr("Projection"));
  _plotCurves.setHeaderData(2,Qt::Horizontal,tr("Magnetic moment"));
  _plotCurves.setHeaderData(3,Qt::Horizontal,tr("Color"));
  _plotCurves.setHeaderData(4,Qt::Horizontal,tr("Label"));
  this->plot();
}

void QDos::on_actionAdd_triggered()
{
    AddDos dialog;
    dialog.setDb(&_db);
    bool soc = _db.atom(_db.list().front()).prtdos()==5;
    if(dialog.exec() == QDialog::Accepted)
    {
      _plotCurves.appendRow(dialog.item());
      _plotCurves.setHeaderData(0,Qt::Horizontal,tr("Selection"));
      _plotCurves.setHeaderData(1,Qt::Horizontal,soc?"SOC":"l");
      _plotCurves.setHeaderData(2,Qt::Horizontal,tr("m"));
      _plotCurves.setHeaderData(3,Qt::Horizontal,tr("Color"));
      _plotCurves.setHeaderData(4,Qt::Horizontal,tr("Label"));
    }
    this->updateAgateCommand();
}

void QDos::on_buttonBox_clicked(QAbstractButton *button)
{
  QDialogButtonBox::StandardButton b = ui->buttonBox->standardButton(button);
  if ( b == QDialogButtonBox::Save )
  {
      QString filename = QFileDialog::getSaveFileName(this,tr("PDF file"),_currentFolder,"PDF (*.pdf)",nullptr,QFileDialog::DontUseNativeDialog);
      if (!filename.endsWith(".pdf")) filename += ".pdf";
      ui->plot->savePdf(filename,0,0,QCP::epNoCosmetic);
      int pos = filename.lastIndexOf(QRegExp("[/\\\\]"));
      _currentFolder= filename.left(pos+1);
  }
  else if ( b == QDialogButtonBox::Reset)
  {
    _plotCurves.clear();
    ui->command->text().clear();
  }
  else if  (b ==QDialogButtonBox::Apply )
  {
    this->plot();
  }
  else if ( b == QDialogButtonBox::Close )
    this->close();
}

void QDos::on_actionRemove_triggered()
{
    QItemSelectionModel *select = ui->toDraw->selectionModel();
    if (select->hasSelection())
    {
      QModelIndexList modelIndexList = select->selectedRows();
      for (int i = modelIndexList.size()-1; i>=0; --i)
        _plotCurves.removeRow(modelIndexList[i].row());
      /*
      for (auto it = modelIndexList.rbegin(); it != modelIndexList.rend(); ++it)
      {
        _plotCurves.removeRow(it->row());
      }
      */
    }
    this->updateAgateCommand();
}

void QDos::updateAgateCommand()
{
  QString command = ":plot dos ";
  QString prefix=ui->folder->text()+"/"+ui->prefix->text();
  command += "prefix="+prefix.replace(" ","\\ ");
  if (ui->spin->isEnabled() && ui->spin->currentIndex()>0)
    command += " spin="+ui->spin->currentData().toString();
  if (ui->updown->isEnabled() && ui->updown->isChecked())
    command += " updown";
  if (ui->paw->isEnabled())
    command += " paw="+ui->paw->currentData().toString();
  command += " eunit="+QString::fromStdString(ui->unit->currentUnit().str());
  if (!ui->xmin->text().isEmpty() && !ui->xmax->text().isEmpty())
    command += " xrange="+QString::number(ui->xmin->text().toDouble())+":"+QString::number(ui->xmax->text().toDouble());
  if (!ui->ymin->text().isEmpty() && !ui->ymax->text().isEmpty())
    command += " yrange="+QString::number(ui->ymin->text().toDouble())+":"+QString::number(ui->ymax->text().toDouble());

  int nrows = _plotCurves.rowCount();
  if (nrows>0)
  {
    QString projection(" projection=");
    QString soc(" soc=");

    for (int row = 0; row < nrows; ++row)
    {
      QStandardItem *selection = _plotCurves.item(row,0);
      if (!selection->data(Qt::CheckStateRole).toBool()) continue;

      int iatom = selection->data(Qt::UserRole).toInt();
      QString &str = (iatom==0?soc:projection);

      QString proj;
      if (iatom!=0) proj = selection->data(Qt::UserRole).toString();

      for (int col=1; col<3; ++col)
      {
        QStandardItem *prop = _plotCurves.item(row,col);
        QString p=prop->data(Qt::UserRole).toString();
        if (!p.isEmpty())
          proj += (iatom!=0?":":"")+p;
      }

      QStandardItem *colorItem = _plotCurves.item(row,3);
      QColor color(colorItem->data(Qt::DisplayRole).toString());
      if (color.isValid())
      {
        proj += QString(":%0-%1-%2").arg(color.red()).arg(color.green()).arg(color.blue());
      }

      QStandardItem *labelItem= _plotCurves.item(row,4);
      if (!labelItem->data(Qt::DisplayRole).toString().isEmpty())
      {
        proj += ":\""+labelItem->data(Qt::DisplayRole).toString().replace(" ","_")+"\"";
      }
      str += proj+",";
    }
    projection.chop(1);
    soc.chop(1);
    if (projection.size()>11)
      command += projection;
    if (soc.size()>4)
      command += soc;
  }
  ui->command->setText(command);
}

void QDos::plot()
{
  this->setCursor(Qt::WaitCursor);
  try
  {
    ui->statusBar->showMessage(tr("Plot in progress"));
    this->updateAgateCommand();
    ConfigParser parser;
    parser.setContent(ui->command->text().toStdString());
    Graph::plotDOS(_db,parser,ui->plot,Graph::NONE);
    ui->statusBar->clearMessage();
  }
  catch(Exception &e)
  {
    ui->statusBar->showMessage(QString::fromStdString(e.what()));
    QMessageBox::critical(this,tr("Error"),QString::fromStdString(e.fullWhat()));
  }
  this->setCursor(Qt::ArrowCursor);
}

void QDos::on_spin_currentIndexChanged(int index)
{
  ui->updown->setEnabled(index==0);
}

void QDos::adjustXRange(const QCPRange &newRange)
{
  ui->xmin->setText(QString::number(newRange.lower));
  ui->xmax->setText(QString::number(newRange.upper));
}

void QDos::adjustYRange(const QCPRange &newRange)
{
  ui->ymin->setText(QString::number(newRange.lower));
  ui->ymax->setText(QString::number(newRange.upper));
}
