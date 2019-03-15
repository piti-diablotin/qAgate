#include "qconducti.h"
#include "ui_qconducti.h"
#include "conducti/conducti.hpp"
#include <QMimeData>
#include <QString>
#include <QUrl>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

QConducti::QConducti(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QConducti),
  _abiopt(),
  _conducti(),
  _units(),
  _config(),
  _sstream(this),
  _oldStreambuf()
{
  ui->setupUi(this);
  this->on_selectionBox_stateChanged(Qt::Unchecked);
  ui->filename->setText(tr("No file loaded"));
  ui->leftFrame->setDisabled(true);
  ui->progressBar->setDisabled(true);
  this->updateInfo();
  _units[sigmaUnit] = UnitConverter(UnitConverter::pOhmpcm);
  _units[sigmaUnit].rebase(UnitConverter::au);
  _units[sigmaUnit] = UnitConverter::au;
  QConducti::setGraphs();
  QConducti::plot();
  _oldStreambuf[0] = std::cout.rdbuf();
  _oldStreambuf[1] = std::clog.rdbuf();
  _oldStreambuf[2] = std::cerr.rdbuf();
  std::clog.rdbuf(&_sstream);
  _sstream.setObjectName("sstream");
  connect(&_sstream,SIGNAL(overflowed()),this,SLOT(sstream_overflowed()));
  connect(&_sstream,SIGNAL(synchronized()),this,SLOT(sstream_synchronized()));
  connect(&_sstream,SIGNAL(xsputned()),this,SLOT(sstream_xsputned()));
  connect(ui->sigma,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(coordSigmaStatusBar(QMouseEvent*)));
  connect(ui->histogram,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(coordHistogramStatusBar(QMouseEvent*)));
  ui->buttonBox->button(QDialogButtonBox::Save)->setText(tr("&Save"));
  ui->buttonBox->button(QDialogButtonBox::Apply)->setText(tr("&Apply"));
  ui->buttonBox->button(QDialogButtonBox::Close)->setText(tr("&Close"));
  ui->buttonBox->button(QDialogButtonBox::Open)->setText(tr("&Open"));
}

QConducti::~QConducti()
{
  std::cout.rdbuf(_oldStreambuf[0]);
  std::clog.rdbuf(_oldStreambuf[1]);
  std::cerr.rdbuf(_oldStreambuf[2]);
  delete ui;
}

void QConducti::dragEnterEvent(QDragEnterEvent *dragEnterEvent)
{
  if (dragEnterEvent->mimeData()->hasFormat("text/uri-list"))
    dragEnterEvent->acceptProposedAction();
}

void QConducti::dropEvent(QDropEvent *dropEvent)
{
  QList<QUrl> urls = dropEvent->mimeData()->urls();
  if (urls.isEmpty())
    return;

  QString fileName = urls.first().toLocalFile();
  if (fileName.isEmpty())
    return;
  this->openFile(fileName);
}

void QConducti::openFile(QString &fileName)
{
  try {
    ui->statusBar->showMessage(tr("Loading file ")+fileName);
    ui->leftFrame->setDisabled(true);
    _abiopt.readFromFile(fileName.toStdString());
    size_t pos = fileName.toStdString().find_last_of("/\\");
    ui->filename->setText(QString::fromStdString(fileName.toStdString().substr(pos+1)));
    ui->leftFrame->setDisabled(false);
    this->updateInfo();
    _config[0].y.clear();
    _config[0].x.clear();
    _config[0].labels.clear();
    _config[1].y.clear();
    _config[1].x.clear();
    _config[1].labels.clear();
    QConducti::plot();
    ui->progressBar->setEnabled(true);

    int mband = _abiopt.mband();
    ui->bfmin->setRange(1,mband/2-1);
    ui->bfmax->setRange(2,mband/2);
    ui->btmin->setRange(mband/2+1,mband-1);
    ui->btmax->setRange(mband/2+2,mband);

    ui->bfmax->setValue(mband/2);
    ui->btmax->setValue(mband);
    ui->btmin->setValue(mband/2+1);
    this->setEnergyRange(true);

    ui->spinBox->setEnabled(_abiopt.nsppol()==2);
    ui->spinBox->setChecked(_abiopt.nsppol()==2);
    ui->statusBar->showMessage(tr("Ready"));
  }
  catch ( Exception &e ) {
    QMessageBox::critical(this,tr("Error"),QString::fromStdString(e.fullWhat()));
  }
}

void QConducti::on_selectionBox_stateChanged(int arg1)
{
  bool setting = false;
  switch(arg1)
    {
    case Qt::Unchecked:
      setting = false;
      break;
    case Qt::Checked:
      setting = true;
      break;
    default:
      return;
    }
  ui->bandButton->setEnabled(setting);
  ui->energyButton->setEnabled(setting);
  ui->energy->setEnabled(setting);
  ui->band->setEnabled(setting);
  ui->holdSelection->setEnabled(setting);
  if ( arg1 == Qt::Checked )
    {
      if ( !ui->bandButton->isChecked() && !ui->energyButton->isChecked() )
        {
          ui->energyButton->click();
        }
      else if ( ui->bandButton->isChecked() )
        {
          ui->energy->setDisabled(true);
        }
      else if ( ui->energyButton->isChecked() )
        {
          ui->band->setDisabled(true);
        }
    }
}

void QConducti::on_buttonBox_clicked(QAbstractButton *button)
{
  QDialogButtonBox::StandardButton b = ui->buttonBox->standardButton(button);
  if ( b == QDialogButtonBox::Save )
    {
      QString filename = QFileDialog::getSaveFileName(this,tr("Base name"));
      _config[4].save = Graph::DATA;
      _config[5].save = Graph::DATA;
      _config[4].filename = filename.toStdString()+"_sigma";
      _config[5].filename = filename.toStdString()+"_histogram";
      Graph::plot(_config[4], nullptr);
      Graph::plot(_config[5], nullptr);
      _config[4].save = Graph::PRINT;
      _config[5].save = Graph::PRINT;
      Graph::plot(_config[4], ui->sigma);
      Graph::plot(_config[5], ui->histogram);
    }
  else if ( b == QDialogButtonBox::Open )
    {
      QString filename = QFileDialog::getOpenFileName(this,tr("Optic file"));
      if ( !filename.isEmpty() )
        this->openFile(filename);
    }
  else if  (b ==QDialogButtonBox::Apply )
    {
      if ( !ui->holdSigma->isChecked() )
        {
          _config[0].xy.clear();
          _config[0].y.clear();
          _config[0].x.clear();
          _config[0].labels.clear();
          _config[1].xy.clear();
          _config[1].y.clear();
          _config[1].x.clear();
          _config[1].labels.clear();
        }
      if ( !ui->holdSelection->isChecked() )
        {
          _config[2].xy.clear();
          _config[2].y.clear();
          _config[2].x.clear();
          _config[2].labels.clear();
          _config[3].xy.clear();
          _config[3].y.clear();
          _config[3].x.clear();
          _config[3].labels.clear();
        }
      QConducti::computeAll();
      QConducti::setGraphs();
      QConducti::plot();
    }
  else if ( b == QDialogButtonBox::Close )
    this->close();
}

void QConducti::updateInfo()
{
  double max = Conducti::getOmegaMax(_abiopt);
  if ( !std::isnan(max))
    {
      ui->omax->setMaximum(max*_units[omegaUnit]);
      ui->omax->setMinimum(ui->omin->value()+(ui->omax->value()-ui->omin->value())/ui->nomega->value());
      ui->omin->setMaximum(ui->omax->value()-(ui->omax->value()-ui->omin->value())/ui->nomega->value());
    }
  else
    {
      max = 0;
    }
  ui->omegaMax->setText(QString::number(max*_units[energyUnit])
                        +" ["
                        +QString::fromStdString(_units[energyUnit].str())
                        +"]");
  ui->fermi->setText(QString::number(_abiopt.fermie()*_units[energyUnit])
                     +" ["
                     +QString::fromStdString(_units[energyUnit].str())
                     +"]");
}

void QConducti::on_energyCombo_currentIndexChanged(const QString &arg1)
{
  auto newUnit = UnitConverter::getFromString(arg1.toStdString());
  newUnit.rebase(UnitConverter::Ha);
  double factor = 1./_units[energyUnit]*newUnit;
  _units[energyUnit] = UnitConverter::getUnit(arg1.toStdString());
  this->updateInfo();
  for ( int i = 0 ; i < 6 ; ++i )
    {
      for ( auto &pair : _config[i].xy )
        {
          for ( auto& w : pair.first )
            w *= factor;
        }
    }
  QConducti::setGraphs();
  //QConducti::plot();
  Graph::plot(_config[4],ui->sigma);
  Graph::plot(_config[5],ui->histogram);
}

void QConducti::on_rangeUnit_currentIndexChanged(const QString &arg1)
{
  auto newUnit = UnitConverter::getFromString(arg1.toStdString());
  newUnit.rebase(UnitConverter::Ha);
  double v1 = ui->omin->value()/_units[omegaUnit]*newUnit;
  double v2 = ui->omax->value()/_units[omegaUnit]*newUnit;
  ui->omin->setRange(
        0.0001,
        //ui->omin->minimum()/_units[omegaUnit]*newUnit,
        ui->omin->maximum()/_units[omegaUnit]*newUnit);
  ui->omax->setRange(
        ui->omax->minimum()/_units[omegaUnit]*newUnit,
        ui->omax->maximum()/_units[omegaUnit]*newUnit);
  ui->omin->setValue(v1);
  ui->omax->setValue(v2);
  _units[omegaUnit] = newUnit;
}

void QConducti::on_smearingUnit_currentIndexChanged(const QString &arg1)
{
  auto newUnit = UnitConverter::getFromString(arg1.toStdString());
  newUnit.rebase(UnitConverter::Ha);
  double v = ui->smearing->value()/_units[smearingUnit]*newUnit;
  ui->smearing->setRange(
        0.0001,
        //ui->smearing->minimum()/_units[smearingUnit]*newUnit,
        ui->smearing->maximum()/_units[smearingUnit]*newUnit);
  ui->smearing->setValue(v);
  _units[smearingUnit] = newUnit;
}

void QConducti::on_omin_editingFinished()
{
  ui->omax->setMinimum(ui->omin->value()+0.0001);
}

void QConducti::on_omax_editingFinished()
{
  ui->omin->setMaximum(ui->omax->value()-0.0001);
}

void QConducti::on_bandButton_clicked()
{
  ui->energy->setDisabled(!ui->bandButton->isDown());
  ui->band->setDisabled(ui->bandButton->isDown());
}

void QConducti::on_energyButton_clicked()
{
  ui->band->setDisabled(!ui->energyButton->isDown());
  ui->energy->setDisabled(ui->energyButton->isDown());
}

void QConducti::setGraphs()
{
  QString freq = tr("Frequency");
  QString sig = tr("Conductivity");
  QString histo = tr("Histogram");
  _config[4].title = "";
  _config[4].xlabel = freq.toStdString()+" omega ["+ _units[energyUnit].str() +"]";
  _config[4].ylabel = sig.toStdString()+" sigma ["+ _units[sigmaUnit].str() +"]";
  _config[5].title = "";
  _config[5].xlabel = freq.toStdString()+" omega ["+ _units[energyUnit].str() +"]";
  _config[5].ylabel = histo.toStdString();
}

void QConducti::plot()
{
 _config[4].xy.clear();
 _config[5].xy.clear();
 _config[4].labels.clear();
 _config[5].labels.clear();
  for ( int i = 0 ; i < 4 ; ++i )
    {
      //_config[i%2+4].xy.merge(_config[i].xy);
      //_config[i%2+4].labels.merge(_config[i].labels);
      for ( auto it = _config[i].xy.begin(); it != _config[i].xy.end(); ++it )
        {
          _config[i%2+4].xy.push_back(*it);
        }
      for ( auto it = _config[i].labels.begin(); it != _config[i].labels.end(); ++it )
        {
          _config[i%2+4].labels.push_back(*it);
        }
    }
  Graph::plot(_config[4],ui->sigma);
  Graph::plot(_config[5],ui->histogram);
}

void QConducti::computeAll()
{
  if ( !ui->leftFrame->isEnabled() ) return;
  try
  {
  _conducti.setNOmega(ui->nomega->value());
  _conducti.setSmearing(ui->smearing->value()/_units[smearingUnit]);
  _conducti.setOmegaRange(ui->omin->value()/_units[omegaUnit],ui->omax->value()/_units[omegaUnit]);
  _conducti.setUnits(_units[energyUnit].str(),_units[sigmaUnit].str());
  _conducti.traceTensor(_abiopt);
  _conducti.getResultSigma(_config[0],ui->spinBox->isChecked());
  _conducti.getResultHistogram(_config[1]);
  for ( auto it = _config[0].y.begin(); it != _config[0].y.end(); ++it )
    _config[0].xy.push_back(std::make_pair(_config[0].x,*it));
  for ( auto it = _config[1].y.begin(); it != _config[1].y.end(); ++it )
    _config[1].xy.push_back(std::make_pair(_config[1].x,*it));
  _config[0].x.clear();
  _config[1].x.clear();
  _config[0].y.clear();
  _config[1].y.clear();
  if ( ui->selectionBox->isChecked() )
    {
      if ( ui->energyButton->isChecked() )
        {
          _conducti.setRange(ui->efmin->value()/_units[energyRangeUnit],
                             ui->efmax->value()/_units[energyRangeUnit],
                             ui->etmin->value()/_units[energyRangeUnit],
                             ui->etmax->value()/_units[energyRangeUnit]
                             );
        }
      else
        {
          _conducti.setRange(ui->bfmin->value(),
                             ui->bfmax->value(),
                             ui->btmin->value(),
                             ui->btmax->value()
                             );

        }
      _conducti.traceTensor(_abiopt);
      _conducti.getResultSigma(_config[2],ui->spinBox->isChecked());
      _conducti.getResultHistogram(_config[3]);
      for ( auto it = _config[2].y.begin(); it != _config[2].y.end(); ++it )
        _config[2].xy.push_back(std::make_pair(_config[2].x,*it));
      for ( auto it = _config[3].y.begin(); it != _config[3].y.end(); ++it )
        {
        _config[3].xy.push_back(std::make_pair(_config[3].x,*it));

        }
      _config[2].x.clear();
      _config[3].x.clear();
      _config[2].y.clear();
      _config[3].y.clear();
    }
  }
  catch ( Exception& e)
  {
    QMessageBox::critical(this,tr("Error"),QString::fromStdString(e.fullWhat()));
  }
}

void QConducti::on_conductiCombo_currentIndexChanged(const QString &arg1)
{
  auto newUnit = UnitConverter::getFromString(arg1.toStdString());
  newUnit.rebase(UnitConverter::au);
  double factor = 1./_units[sigmaUnit]*newUnit;
  _units[sigmaUnit] = newUnit;
  for ( auto &pair : _config[4].xy )
    {
      for ( auto &s : pair.second )
        s *= factor;
    }
  QConducti::setGraphs();
  //QConducti::plot();
  Graph::plot(_config[4],ui->sigma);
  //Graph::plot(_config[1],ui->histogram);
}

void QConducti::on_selectionBox_toggled(bool checked)
{
  if ( checked )
    {
    }
  else
    {
      _config[2].x.clear();
      _config[2].y.clear();
      _config[2].xy.clear();
      _config[2].labels.clear();
      _config[3].x.clear();
      _config[3].y.clear();
      _config[3].xy.clear();
      _config[3].labels.clear();
      QConducti::plot();
    }
}


void QConducti::sstream_overflowed()
{
  //qDebug() << "STREAM overflowed" ;
}

void QConducti::sstream_synchronized()
{
  //qDebug() << "STREAM synchronized";
}

void QConducti::sstream_xsputned()
{
  bool ok;
  int percent = _sstream.readAll().remove("%").toInt(&ok,10);
  if ( ok )
    ui->progressBar->setValue(percent);
}

void QConducti::on_bfmin_editingFinished()
{
  ui->bfmax->setMinimum(ui->bfmin->value()+1);
}

void QConducti::on_bfmax_editingFinished()
{
  ui->bfmin->setMaximum(ui->bfmax->value()-1);
  ui->btmin->setMinimum(ui->bfmax->value()+1);
}

void QConducti::on_btmin_editingFinished()
{
  ui->btmax->setMinimum(ui->btmin->value()+1);
  ui->bfmax->setMaximum(ui->btmin->value()-1);
}

void QConducti::on_btmax_editingFinished()
{
  ui->btmin->setMaximum(ui->btmax->value()-1);
}

void QConducti::setEnergyRange(bool setValue) {
  double max = Conducti::getOmegaMax(_abiopt);
  if (!std::isnan(max))
    {
      ui->efmin->setRange(-max*_units[energyRangeUnit],-0.0001);
      ui->efmax->setRange((-max*_units[energyRangeUnit]+0.0001),0);
      ui->etmin->setRange(0.0001,(max*_units[energyRangeUnit]-0.0001));
      ui->etmax->setRange(0.0002,max*_units[energyRangeUnit]);
      if (setValue)
        {
          ui->efmin->setValue(-max);
          ui->efmax->setValue(0);
          ui->etmin->setValue(0.0001);
          ui->etmax->setValue(max);
        }
    }
}

void QConducti::on_unit_currentIndexChanged(const QString &arg1)
{
  auto newUnit = UnitConverter::getFromString(arg1.toStdString());
  newUnit.rebase(UnitConverter::Ha);
  double factor = 1./_units[energyRangeUnit]*newUnit;
  _units[energyRangeUnit] = newUnit;
  double efmin = ui->efmin->value()*factor;
  double efmax = ui->efmax->value()*factor;
  double etmin = ui->etmin->value()*factor;
  double etmax = ui->etmax->value()*factor;
  QConducti::setEnergyRange(false);
  ui->efmin->setValue(efmin);
  ui->efmax->setValue(efmax);
  ui->etmin->setValue(etmin);
  ui->etmax->setValue(etmax);
}

void QConducti::on_efmin_editingFinished()
{
  ui->efmax->setMinimum(ui->efmin->value()+0.0001);
}

void QConducti::on_efmax_editingFinished()
{
  ui->efmin->setMaximum(ui->efmax->value()-0.0001);
  ui->etmin->setMinimum(ui->efmax->value()+0.0001);
}

void QConducti::on_etmin_editingFinished()
{
  ui->etmax->setMinimum(ui->etmin->value()+0.0001);
  ui->efmax->setMaximum(ui->etmin->value()-0.0001);
}

void QConducti::on_etmax_editingFinished()
{
  ui->etmin->setMaximum(ui->etmax->value()-0.0001);
}

void QConducti::coordSigmaStatusBar(QMouseEvent *event) {
  double x = ui->sigma->xAxis->pixelToCoord(event->pos().x());
  double y = ui->sigma->yAxis->pixelToCoord(event->pos().y());
  QString info = QString(QChar(0xC9,0x03)) + "="+QString::number(x) + " " + QChar(0xC3,0x03) + "=" + QString::number(y);
  ui->statusBar->showMessage(info);
}

void QConducti::coordHistogramStatusBar(QMouseEvent *event) {
  double x = ui->histogram->xAxis->pixelToCoord(event->pos().x());
  double y = ui->histogram->yAxis->pixelToCoord(event->pos().y());
  QString info = QString(QChar(0xC9,0x03)) + "="+QString::number(x) + " " +QString(tr("Histogram"))+"=" + QString::number(y);
  ui->statusBar->showMessage(info);
}
