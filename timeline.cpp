#include "timeline.h"
#include "ui_timeline.h"

TimeLine::TimeLine(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TimeLine),
  _timeBegin(0),
  _timeEnd(0),
  _time(0),
  _timeTotal(0)
{
  ui->setupUi(this);
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

void TimeLine::on_timeBegin_valueChanged(int time)
{
  auto sizes = ui->splitter->sizes();
  int total = sizes[0]+sizes[1]+sizes[2];
  if ( time >= _timeEnd ) return;
  sizes[0] = total*time/_timeTotal;
  sizes[1] = total-sizes[0]-sizes[2];
  _timeBegin = time;
  set_splitter();
  emit(timeBeginChanged(_timeBegin));
}

void TimeLine::on_slider_valueChanged(int value)
{
  emit(timeChanged(value));
}

void TimeLine::on_timeEnd_valueChanged(int time)
{
  auto sizes = ui->splitter->sizes();
  int total = sizes[0]+sizes[1]+sizes[2];
  if ( time < _timeBegin && time >=_timeTotal ) return;
  _timeEnd = time;
  sizes[2] = total*(_timeTotal-1-_timeEnd)/_timeTotal;
  sizes[1] = total-sizes[0]-sizes[2];
  set_splitter();
  emit(timeEndChanged(_timeEnd));
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
  ui->timeBegin->setValue(time);
}

void TimeLine::setTimeEnd(int time)
{
  ui->timeEnd->setValue(time);
}

void TimeLine::setTime(int time)
{
  ui->slider->setValue(time);
}

void TimeLine::setTimeTotal(int time)
{
  if (time <= _timeEnd) _timeEnd = time-1;
  if (time < _timeBegin) _timeBegin = time-1;
  if (time < _time) _time = time-1;
  _timeTotal = time;
  TimeLine::setTimeBegin(_timeBegin);
  TimeLine::setTimeEnd(_timeEnd);
  TimeLine::setTime(_time);
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
  TimeLine::setTimeBegin(begin);
  TimeLine::setTimeEnd(end);
  TimeLine::setTime(time);
}
