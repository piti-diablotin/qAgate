#include "timeline.h"
#include "ui_timeline.h"
#include <QDebug>
#include <QKeyEvent>

TimeLine::TimeLine(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TimeLine),
  _timeBegin(0),
  _timeEnd(0),
  _time(0),
  _timeTotal(0),
  _autoUpdate(false)
{
  ui->setupUi(this);
  //this->set_splitter();
  ui->timeBegin->setMaximum(_timeTotal);
  ui->timeBegin->setValue(_timeBegin);
  ui->timeEnd->setMaximum(_timeTotal);
  ui->timeEnd->setValue(_timeEnd);
  auto sizes = ui->splitter->sizes();
  sizes[1] += sizes[0]+ sizes[2];
  sizes[0] = 0;
  sizes[2] = 0;
  ui->splitter->setSizes(sizes);
}

TimeLine::~TimeLine()
{
  delete ui;
}

int TimeLine::timeBegin() const
{
  return _timeBegin;
}

int TimeLine::timeEnd() const
{
  return _timeEnd;
}

int TimeLine::time() const
{
  return _time;
}

int TimeLine::timeTotal() const
{
  return _timeTotal;
}

void TimeLine::on_timeBegin_valueChanged(int time)
{
  //auto sizes = ui->splitter->sizes();
  //int total = sizes[0]+sizes[1]+sizes[2];
  if ( time >= _timeEnd ) return;
  //sizes[0] = total*time/_timeTotal;
  //sizes[1] = total-sizes[0]-sizes[2];
  _timeBegin = time;
  set_splitter();
  if ( !_autoUpdate ) emit(timeBeginChanged());
}

void TimeLine::on_slider_valueChanged(int value)
{
  _time = value;
  if ( !_autoUpdate ) emit(timeChanged());
}

void TimeLine::on_timeEnd_valueChanged(int time)
{
  //auto sizes = ui->splitter->sizes();
  //int total = sizes[0]+sizes[1]+sizes[2];
  if ( time < _timeBegin && time >=_timeTotal ) return;
  _timeEnd = time;
  //sizes[2] = total*(_timeTotal-1-_timeEnd)/_timeTotal;
  //sizes[1] = total-sizes[0]-sizes[2];
  set_splitter();
  if ( !_autoUpdate ) emit(timeEndChanged());
}

void TimeLine::on_splitter_splitterMoved(int pos, int index)
{
  (void) pos;
  auto sizes = ui->splitter->sizes();
  int total = sizes[0]+sizes[1]+sizes[2];
  int time;
  switch (index)
    {
    case 1: //timeBegin
      time = (sizes[0]*_timeTotal)/total;
      ui->timeBegin->setValue(time);
      break;
    case 2: // timeEnd
      time = _timeTotal - (sizes[2]*_timeTotal)/total-1;
      ui->timeEnd->setValue(time);
      break;
    default:
      break;
    }
}

void TimeLine::set_splitter()
{
  if (_timeTotal == 0) return;
  ui->timeEnd->setRange(0,_timeTotal-1);
  ui->timeBegin->setRange(0,_timeTotal-1);
  ui->slider->setRange(_timeBegin,_timeEnd);
  auto sizes = ui->splitter->sizes();
  int total = sizes[0]+sizes[1]+sizes[2];
  sizes[0] = total*_timeBegin/_timeTotal;
  sizes[2] = total * (_timeTotal-1 - _timeEnd )/_timeTotal;
  sizes[1] = total-sizes[0]-sizes[2];
  ui->splitter->setSizes(sizes);
}

void TimeLine::setTimeBegin(int time)
{
  _autoUpdate = true;
  ui->timeBegin->setValue(time);
  _autoUpdate = false;
}

void TimeLine::setTimeEnd(int time)
{
  _autoUpdate = true;
  ui->timeEnd->setValue(time);
  _autoUpdate = false;
}

void TimeLine::setTime(int time)
{
  _autoUpdate = true;
  ui->slider->setValue(time);
  _autoUpdate = false;
}

void TimeLine::setTimeTotal(int time)
{
  _autoUpdate = true;
  if (time < 1) return;
  if (time <= _timeEnd) _timeEnd = time-1;
  if (time < _timeBegin) _timeBegin = time-1;
  if (time < _time) _time = time-1;
  _timeTotal = time;
  set_splitter();
  _autoUpdate = false;
  //TimeLine::setTimeBegin(_timeBegin);
  //TimeLine::setTimeEnd(_timeEnd);
  //TimeLine::setTime(_time);
}

void TimeLine::setTimes(int begin, int end, int time, int total)
{
  if ( !(begin <= time && time <= end && end < total) )
    {
      if ( end >= total ) end = total-1;
      if ( time > end ) time = end;
      if ( begin > end ) begin = end;
      if ( time < begin ) time = begin;
      if ( begin < 0 ) begin = 0;
    }
  _timeTotal = total;
  _timeBegin = begin;
  _timeEnd = end;
  _time = time;
  set_splitter();
  TimeLine::setTimeBegin(begin);
  TimeLine::setTimeEnd(end);
  TimeLine::setTime(time);
}
