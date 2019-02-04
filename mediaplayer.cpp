#include "mediaplayer.h"
#include "ui_mediaplayer.h"
#include <QFileDialog>

MediaPlayer::MediaPlayer(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MediaPlayer),
  _repeat(None),
  _currentFolder(".")
{
  ui->setupUi(this);
}

MediaPlayer::~MediaPlayer()
{
  delete ui;
}

void MediaPlayer::setDisabled(bool state)
{
  ui->append->setDisabled(state);
  MediaPlayer::setDisabled(state);
}

void MediaPlayer::setDisabledMovie(bool state)
{
  ui->play->setEnabled(state);
  ui->next->setEnabled(state);
  ui->previous->setEnabled(state);
  ui->faster->setEnabled(state);
  ui->slower->setEnabled(state);
  ui->repeat->setEnabled(state);
  ui->record->setEnabled(state);
}

void MediaPlayer::setPlay(bool state)
{
  if (ui->play->isChecked() ^ state) ui->play->toggle();
}

void MediaPlayer::setRecord(bool state)
{
  if (ui->record->isChecked() ^ state) ui->record->toggle();
}

void MediaPlayer::setRepeatMode(MediaPlayer::RepeatMode mode)
{
  switch (mode) {
    case None:
      if (ui->repeat->isChecked())
        {
          ui->repeat->toggle();
          ui->repeat->setIcon(QIcon(":/mplayer/icons/rotate-cw.svg"));
        }
      break;
    case Repeat:
      if (!ui->repeat->isChecked())
        {
          ui->repeat->toggle();
        }
      ui->repeat->setIcon(QIcon(":/mplayer/icons/rotate-cw.svg"));
      break;
    case Palindrome:
      if (!ui->repeat->isChecked())
        {
          ui->repeat->toggle();
        }
      ui->repeat->setIcon(QIcon(":/mplayer/icons/repeat.svg"));
      break;
    default:
      break;
    }
  _repeat = mode;
}

void MediaPlayer::setDisabledAppend(bool state)
{
  ui->append->setDisabled(state);
}

void MediaPlayer::on_play_clicked(bool checked)
{
  if (checked)
    {
      emit(play());
    }
  else
    {
      emit(pause());
    }
}

void MediaPlayer::on_play_toggled(bool checked)
{
  if (checked)
    {
      ui->play->setIcon(QIcon(":/mplayer/icons/pause.svg"));
      ui->play->setToolTip(tr("Pause"));
      ui->play->setWhatsThis(tr("Pause the movie"));
      ui->play->setStatusTip(tr("Pause"));
    }
  else
    {
      ui->play->setIcon(QIcon(":/mplayer/icons/play.svg"));
      ui->play->setToolTip(tr("Play"));
      ui->play->setWhatsThis(tr("Play the movie"));
      ui->play->setStatusTip(tr("Play"));
    }
}

void MediaPlayer::on_repeat_clicked(bool checked)
{
  (void) checked;
  // clicked is called after toggled
  switch (_repeat) {
    case None:
      _repeat = Repeat;
      ui->repeat->setIcon(QIcon(":/mplayer/icons/rotate-cw.svg"));
      break;
    case Repeat:
      ui->repeat->toggle();
      _repeat = Palindrome;
      ui->repeat->setIcon(QIcon(":/mplayer/icons/repeat.svg"));
      break;
    case Palindrome:
      _repeat = None;
      ui->repeat->setIcon(QIcon(":/mplayer/icons/rotate-cw.svg"));
      break;
    default:
      break;
    }
  emit(repeat(_repeat));
}

void MediaPlayer::on_record_toggled(bool checked)
{
   if (checked)
     {
       ui->record->setIcon(QIcon(":/mplayer/icons/video-off.svg"));
     }
   else
     {
       ui->record->setIcon(QIcon(":/mplayer/icons/video.svg"));
     }
}

void MediaPlayer::on_record_clicked(bool checked)
{
   if (checked)
     {
       emit(record(true));
     }
   else
     {
       emit(record(false));
     }
}

void MediaPlayer::on_previous_clicked()
{
  emit(previous());
}

void MediaPlayer::on_next_clicked()
{
  emit(next());
}

void MediaPlayer::on_slower_clicked()
{
   emit(slower());
}

void MediaPlayer::on_faster_clicked()
{
   emit(faster());
}

void MediaPlayer::on_snapshot_clicked()
{
   emit(snapshot());
}

void MediaPlayer::on_open_clicked()
{
  auto fileNames = QFileDialog::getOpenFileNames(this,"Open File",_currentFolder,"Abinit (*.in *.out *_OUT.nc *_HIST *_HIST.nc *_DDB *_DEN *_OPT);;VASP (POSCAR);;CIF (*.cif);;XML (*.xml);;XYZ (*.xyz);; YAML(*.yaml);;All (*)");
  if ( !fileNames.empty() )
    {
      QString file1 = fileNames.first();
      if (file1.isEmpty())
        return;
      emit(open(file1));
      int pos = file1.lastIndexOf(QRegExp("[/\\\\]"));
      _currentFolder = file1.left(pos+1);

      for ( auto file = fileNames.begin()+1 ; file != fileNames.end() ; ++file )
        {
          if ( !file->isEmpty() )
            {
              emit(append(*file));
            }
        }
    }
}

void MediaPlayer::on_append_clicked()
{
  auto fileNames = QFileDialog::getOpenFileNames(this,"Append File",_currentFolder,"Abinit (*.in *.out *_OUT.nc *_HIST *_HIST.nc *_DDB *_DEN *_OPT);;VASP (POSCAR);;CIF (*.cif);;XML (*.xml);;XYZ (*.xyz);; YAML (*.yaml) All (*)");

  if ( !fileNames.empty() )
    {
      for ( auto file = fileNames.begin() ; file != fileNames.end() ; ++file )
        {
          if ( !file->isEmpty() )
            {
              emit(append(*file));
              int pos = file->lastIndexOf(QRegExp("[/\\\\]"));
              _currentFolder = file->left(pos+1);
            }
        }
    }

}

void MediaPlayer::on_zoomOut_clicked()
{
  emit(zoomOut());
}

void MediaPlayer::on_zoomIn_clicked()
{
  emit(zoomIn());
}
