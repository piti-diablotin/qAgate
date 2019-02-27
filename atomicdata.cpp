#include "atomicdata.h"
#include "ui_atomicdata.h"
#include <QDebug>


atomicData::atomicData(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::atomicData)
{
  ui->setupUi(this);
}

atomicData::~atomicData()
{
  delete ui;
}

void atomicData::set(int z, QString name, double mass, double radius, double rcov, QColor color)
{
  ui->z->setText(QString::number(z));
  ui->name->setText(name.trimmed());
  ui->mass->setText(QString::number(mass));
  ui->radius->setText(QString::number(radius));
  ui->rcov->setText(QString::number(rcov));
  _mass = mass;
  _radius = radius;
  _rcov = rcov;
  _color = color;
  this->setStyleSheet("background-color:"+_color.name()+";");
}

int atomicData::z() const
{
  return ui->z->text().toInt();
}

QString atomicData::name() const
{
  return ui->name->text();
}

double atomicData::mass() const
{
  return _mass;
}

double atomicData::radius() const
{
  return _radius;
}

double atomicData::rcov() const
{
  return _rcov;
}

QColor atomicData::color() const
{
  return _color;
}

void atomicData::setMass(double mass)
{
 _mass = mass;
 ui->mass->setText(QString::number(mass));
}

void atomicData::setRadius(double radius)
{
_radius = radius;
ui->radius->setText(QString::number(radius));
}

void atomicData::setRcov(double rcov)
{
  _rcov = rcov;
  ui->rcov->setText(QString::number(rcov));
}

void atomicData::setColor(QColor color)
{
  _color = color;
  this->setStyleSheet("background-color:"+_color.name()+";");
}

void atomicData::mousePressEvent(QMouseEvent* mouseEvent)
{
  if (mouseEvent->button()==Qt::LeftButton)
    emit(editElement(this));
}
