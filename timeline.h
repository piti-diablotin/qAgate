#ifndef TIMELINE_H
#define TIMELINE_H

#include <QWidget>

namespace Ui {
  class TimeLine;
}

class TimeLine : public QWidget
{
  Q_OBJECT

public:
  explicit TimeLine(QWidget *parent = 0);
  ~TimeLine();
  int timeBegin() const;
  int timeEnd() const;
  int time() const;
  int timeTotal() const;

private slots:
  void set_splitter();

  void on_timeBegin_valueChanged(int time);

  void on_slider_valueChanged(int time);

  void on_timeEnd_valueChanged(int time);

  void on_splitter_splitterMoved(int pos, int index);

public slots:
  void setTimeBegin(int time);
  void setTimeEnd(int time);
  void setTime(int time);
  void setTimeTotal(int time);
  void setTimes(int begin, int end, int time, int total);

private:
  Ui::TimeLine *ui;
  int _timeBegin;
  int _timeEnd;
  int _time;
  int _timeTotal;
  bool _autoUpdate;

signals:
  void timeBeginChanged();
  void timeEndChanged();
  void timeChanged();
};

#endif // TIMELINE_H
