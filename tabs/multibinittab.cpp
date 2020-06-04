#include "multibinittab.h"
#include "ui_multibinittab.h"
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegExpValidator>
#include "hist/histcustommodes.hpp"
#include "canvas/canvasphonons.hpp"
#include <QMessageBox>

MultibinitTab::MultibinitTab(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::MultibinitTab)
{
  ui->setupUi(this);
  ui->seed->setValidator(new QIntValidator(this));
  ui->amplitude->setValidator(new QDoubleValidator(0.1,10,10,this));
  ui->supercell->setValidator(new QRegExpValidator(QRegExp("^(\\d+) (\\d+) (\\d+)$")));
  ui->seedType->setItemData(0,"random");
  ui->seedType->setItemData(1,"user");
  ui->seedType->setItemData(2,"time");
  ui->seedType->setItemData(3,"none");
  ui->instable->setItemData(0,"ignore");
  ui->instable->setItemData(0,"ignore");
  ui->instable->setItemData(1,"constant");
  ui->instable->setItemData(2,"absolute");
}

MultibinitTab::~MultibinitTab()
{
  delete ui;
}

void MultibinitTab::updateStatus(View* view)
{
  const CanvasPhonons* canvas = dynamic_cast<const CanvasPhonons*>(view->getCanvas());
  this->setEnabled(canvas!=nullptr);
  /*
  if (canvas!=nullptr)
  {
    const HistCustomModes *hist = dynamic_cast<const HistCustomModes*>(canvas->histdata());
    if (hist!=nullptr)
    {
      ui->amplitude->setText(QString::number(hist->instableAmplitude()));
      ui->seed->setText(QString::number(hist->seed()));
      ui->seedType->setCurrentIndex(ui->seedType->findData(hist->seedType()));
      ui->amplitude->setText(QString::number(hist->instableAmplitude()));
      ui->temperature->setValue((int)hist->temperature());
    }
  }
  */
}

void MultibinitTab::on_seedType_currentIndexChanged(int index)
{
  bool enable = (index==1);
  ui->seed->setEnabled(enable);
  ui->seedLabel->setEnabled(enable);
}

void MultibinitTab::on_instable_currentIndexChanged(int index)
{
  bool enable = (index==1);
  ui->amplitude->setEnabled(enable);
  ui->amplitudeLabel->setEnabled(enable);
}

void MultibinitTab::on_generate_clicked()
{

  QString command;
  if (ui->randomHist->isChecked())
  {
    if (ui->supercell->text().isEmpty())
    {
      QMessageBox::information(this,tr("Missing data"),tr("You need to give the size of the supercell as X X X"));
      return;
    }
    command = ":thermalPop temperature=%0 ntime=%1 qpt=%2 seedType=%3 instable=%4";
    command = command.arg(ui->temperature->value()).arg(ui->quantity->value()).arg(ui->supercell->text(),ui->seedType->currentData().toString(),ui->instable->currentData().toString());
  }
  else {
    if ( ui->trajHist->text().isEmpty() )
    {
      QMessageBox::information(this,tr("Missing data"),tr("You need to provide a file"));
      return;
    }
    command = ":thermalPop temperature=%0 trajectory=%1 seedType=%2 instable=%3";
    auto filename = ui->trajFile->text().replace(" ","\\ ");
    command = command.arg(ui->temperature->value()).arg(filename).arg(ui->seedType->currentData().toString(),ui->instable->currentData().toString());
  }
  if (ui->seedType->currentIndex()==1)
  {
    if (ui->seed->text().isEmpty())
    {
      QMessageBox::information(this,tr("Missing data"),tr("You need to give the seed"));
      return;
    }
    command += QString(" seed=%0").arg(QString::number(ui->seed->text().toInt()));
  }
  if (ui->instable->currentIndex()==1)
  {
    if (ui->amplitude->text().isEmpty())
    {
      QMessageBox::information(this,tr("Missing data"),tr("You need to give the amplitude for instable modes"));
      return;
    }
    command += QString(" instableAmplitude=%0").arg(QString::number(ui->amplitude->text().toDouble()));
  }
  emit(sendCommand(command));
}

void MultibinitTab::on_randomHist_clicked(bool checked)
{
  ui->supercell->setEnabled(checked);
  ui->supercellLabel->setEnabled(checked);
  ui->quantity->setEnabled(checked);
  ui->quantityLabel->setEnabled(checked);
  ui->trajFile->setDisabled(checked);
  ui->browse->setDisabled(checked);
  ui->pump->setDisabled(checked);
}

void MultibinitTab::on_trajHist_clicked(bool checked)
{
  ui->supercell->setDisabled(checked);
  ui->supercellLabel->setDisabled(checked);
  ui->quantity->setDisabled(checked);
  ui->quantityLabel->setDisabled(checked);
  ui->trajFile->setEnabled(checked);
  ui->browse->setEnabled(checked);
  ui->pump->setEnabled(checked);
}

void MultibinitTab::on_browse_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),_currentFolder,"Abinit (*.in *.out *_OUT.nc *_HIST *_HIST.nc *_DDB *_DEN *_OPT);;VASP (POSCAR);;CIF (*.cif);;XML (*.xml);; YAML(*.yaml);;All (*)",nullptr,QFileDialog::DontUseNativeDialog);
  if ( !fileName.isEmpty() )
  {
    ui->trajFile->setText(fileName);
    int pos = fileName.lastIndexOf(QRegExp("[/\\\\]"));
    _currentFolder = fileName.left(pos+1);
    HistData* tmp = HistData::getHist(fileName.toStdString());
    ui->time->setMaximum(tmp->ntime()-1);
    ui->time->setSuffix(QString("/%0").arg(tmp->ntime()-1));
    delete tmp;
  }
}

void MultibinitTab::on_pumpButton_clicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), _currentFolder,"",nullptr,QFileDialog::DontUseNativeDialog);
  if (!fileName.isEmpty())
  {
    emit(sendCommand(QString(":pump structure=%0 time=%1 output=%2").arg(ui->trajFile->text(),QString::number(ui->time->value()), fileName.replace(" ","\\ "))));
    int pos = fileName.lastIndexOf(QRegExp("[/\\\\]"));
    _currentFolder = fileName.left(pos+1);
  }
}
