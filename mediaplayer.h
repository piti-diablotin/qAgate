#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QWidget>

namespace Ui {
  class MediaPlayer;
}

class MediaPlayer : public QWidget
{
  Q_OBJECT

public:
  enum RepeatMode {None,Repeat,Palindrome};
  explicit MediaPlayer(QWidget *parent = 0);
  ~MediaPlayer();

private:
  Ui::MediaPlayer *ui;
  RepeatMode _repeat;
  QString _currentFolder;

signals:
  void play();
  void pause();
  void norepeat();
  void repeat();
  void palindrome();
  void next();
  void previous();
  void faster();
  void slower();
  void snapshot();
  void record();
  void stopRecord();
  void open(QString);
  void append(QString);
  void zoomIn();
  void zoomOut();

public slots:
  //void setDisabled(bool state);
  void setDisabledMovie(bool state);
  void setPlay(bool state);
  void setRecord(bool state);
  void setRepeatMode(RepeatMode mode);
  void setDisabledAppend(bool state);

private slots:
  void on_play_clicked(bool checked);
  void on_play_toggled(bool checked);
  void on_repeat_clicked(bool checked);
  void on_record_toggled(bool checked);
  void on_record_clicked(bool checked);
  void on_previous_clicked();
  void on_next_clicked();
  void on_slower_clicked();
  void on_faster_clicked();
  void on_snapshot_clicked();
  void on_open_clicked();
  void on_append_clicked();
  void on_zoomOut_clicked();
  void on_zoomIn_clicked();
};

#endif // MEDIAPLAYER_H
