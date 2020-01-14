#include "localmode.h"
#include "ui_localmode.h"
#include "canvas/canvaslocal.hpp"
#include "base/mendeleev.hpp"

LocalMode::LocalMode(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::LocalMode)
{
  ui->setupUi(this);
}

LocalMode::~LocalMode()
{
  delete ui;
}

void LocalMode::updateStatus(View *view)
{
  // Octahedra
  const CanvasLocal* canvas = dynamic_cast<const CanvasLocal*>(view->getCanvas());
  this->setEnabled(canvas!=nullptr);
  if (canvas!=nullptr)
  {
    const HistData* hist = canvas->histdata();
    if ((hist)!=nullptr)
    {
      ui->octaAdd->clear();
      ui->octaDel->clear();
      ui->octaAdd->addItem(QIcon(":/visuals/icons/plus-circle.svg"),"");
      ui->octaDel->addItem(QIcon(":/visuals/icons/minus-circle.svg"),"");
      bool draw;
      auto octa = canvas->getOctahedra(draw);
      auto znucl = hist->znucl();
      _znuclOrder = QVector<int>::fromStdVector(znucl);
      for (unsigned i = 0; i < znucl.size(); ++i)
      {
        auto found = std::find(octa.begin(),octa.end(),znucl[i]);
        QVariant data(znucl[i]);
        if (found == octa.end())
        {
          int newPos = 1;
          for (newPos=1 ; newPos<ui->octaAdd->count(); ++newPos)
            if (ui->octaAdd->itemData(newPos)>data) break;
          ui->octaAdd->insertItem(newPos,QString(MendeTable.name[znucl[i]]),data);
        }
        else
        {
          int newPos = 1;
          for (newPos=1 ; newPos<ui->octaDel->count(); ++newPos)
            if (ui->octaDel->itemData(newPos)>data) break;
          ui->octaDel->insertItem(newPos,QString(MendeTable.name[znucl[i]]),data);
        }
      }
    }
    const float* colors = canvas->octacolor();
    QPalette pal = ui->colorMinus->palette();
    _minusColor.setRgbF(colors[0],colors[1],colors[2]);
    pal.setColor(QPalette::Button,_minusColor);
    ui->colorMinus->setPalette(pal);
    pal= ui->colorPlus->palette();
    _plusColor.setRgbF(colors[3],colors[4],colors[5]);
    pal.setColor(QPalette::Button,_plusColor);
    ui->colorPlus->setPalette(pal);
    switch(canvas->view())
    {
      case CanvasLocal::ANGLES:
        ui->rotation->setChecked(true);
            break;
      case CanvasLocal::LENGTHS:
        ui->length->setChecked(true);
            break;
    }
    bool baseCart = canvas->baseCart();
    ui->cart->setChecked(baseCart);
    ui->rel->setChecked(!baseCart);
  }
}

void LocalMode::on_octaAdd_activated(int index)
{
  if (index==0) return;
  QString command=":octa_z "+ui->octaAdd->itemText(index);
  auto data = ui->octaAdd->itemData(index);
  int newPos = 1;
  for (newPos=1 ; newPos<ui->octaDel->count(); ++newPos)
    if (ui->octaDel->itemData(newPos)>data) break;
  ui->octaDel->insertItem(newPos,ui->octaAdd->itemText(index),data);
  ui->octaAdd->removeItem(index);
  ui->octaAdd->setCurrentIndex(0);
  emit(sendCommand(command));
}

void LocalMode::on_octaDel_activated(int index)
{
  if (index==0) return;
  QString command=":octa_z -"+ui->octaDel->itemText(index);
  auto data = ui->octaDel->itemData(index);
  int newPos = 1;
  for (newPos=1 ; newPos<ui->octaAdd->count(); ++newPos)
    if (ui->octaAdd->itemData(newPos)>data) break;
  ui->octaAdd->insertItem(newPos,ui->octaDel->itemText(index),data);
  ui->octaDel->removeItem(index);
  ui->octaDel->setCurrentIndex(0);
  emit(sendCommand(command));
}

void LocalMode::on_displayGroup_buttonClicked(QAbstractButton *button)
{
  emit(sendCommand(":local "+button->objectName()));
}

void LocalMode::on_basisGroup_buttonClicked(QAbstractButton *button)
{
  emit(sendCommand(":basis "+button->objectName()));
}

void LocalMode::on_colorPlus_clicked()
{
  QColor newColor = QColorDialog::getColor(_plusColor,this,tr("Pick a color for Clockwise rotations"),QColorDialog::DontUseNativeDialog);
  if (!newColor.isValid()) return;
  _plusColor = newColor;
  QPalette pal = ui->colorPlus->palette();
  pal.setColor(QPalette::Button,_plusColor);
  ui->colorPlus->setPalette(pal);
  QString color = QString::number(newColor.red())
      + " " + QString::number(newColor.green())
      + " " + QString::number(newColor.blue());
  emit(sendCommand(":color plus "+color));
}

void LocalMode::on_colorMinus_clicked()
{
  QColor newColor = QColorDialog::getColor(_minusColor,this,tr("Pick a color for anticlockwise rotations"),QColorDialog::DontUseNativeDialog);
  if (!newColor.isValid()) return;
  _minusColor= newColor;
  QPalette pal = ui->colorMinus->palette();
  pal.setColor(QPalette::Button,_minusColor);
  ui->colorMinus->setPalette(pal);
  QString color = QString::number(newColor.red())
      + " " + QString::number(newColor.green())
      + " " + QString::number(newColor.blue());
  emit(sendCommand(":color minus "+color));
}

void LocalMode::on_exportRotations_clicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), _currentFolder,"",nullptr,QFileDialog::DontUseNativeDialog);
  if (!fileName.isEmpty())
  {
    emit(sendCommand(QString(":rot ")+fileName));
  }
}

void LocalMode::on_exportLengths_clicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), _currentFolder,"",nullptr,QFileDialog::DontUseNativeDialog);
  if (!fileName.isEmpty())
  {
    emit(sendCommand(QString(":length ")+fileName));
  }
}
