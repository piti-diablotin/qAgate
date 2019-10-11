#include "multibinit.h"
#include "ui_multibinit.h"
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegExpValidator>
#include "hist/histcustommodes.hpp"
#include "canvas/canvasphonons.hpp"
#include <QMessageBox>

Multibinit::Multibinit(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::Multibinit)
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

Multibinit::~Multibinit()
{
  delete ui;
}

void Multibinit::updateStatus(View* view)
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

void Multibinit::on_seedType_currentIndexChanged(int index)
{
  bool enable = (index==1);
  ui->seed->setEnabled(enable);
  ui->seedLabel->setEnabled(enable);
}

void Multibinit::on_instable_currentIndexChanged(int index)
{
  bool enable = (index==1);
  ui->amplitude->setEnabled(enable);
  ui->amplitudeLabel->setEnabled(enable);
}

void Multibinit::on_generate_clicked()
{
  if (ui->supercell->text().isEmpty())
  {
    QMessageBox::information(this,tr("Missing data"),tr("You need to give the size of the supercell as X X X"));
    return;
  }

  QString command(":thermalPop temperature=%0 ntime=%1 qpt=%2 seedType=%3 instable=%4");
  command = command.arg(ui->temperature->value()).arg(ui->quantity->value()).arg(ui->supercell->text(),ui->seedType->currentData().toString(),ui->instable->currentData().toString());
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
