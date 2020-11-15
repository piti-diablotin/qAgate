#include "logger.h"
#include "ui_logger.h"
#include <iostream>
#include <QMetaMethod>
#include "base/exception.hpp"
#include "base/utils.hpp"
#include <QClipboard>
#include <QDebug>

Logger::Logger(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Logger),
  _isRead(0),
  _cout(this),
  _clog(this),
  _cerr(this),
  _oldStreambuf(),
  _synchro{-1}
{
  ui->setupUi(this);
  ui->logger->hide();
  ui->info->hide();
  ui->warning->hide();
  ui->error->hide();
#ifdef __APPLE__
  QFont font;
  font.setFamily(QString::fromUtf8("Monaco"));
  font.setPointSize(9);
  ui->logger->setFont(font);
#endif
  _oldStreambuf[0] = std::cout.rdbuf();
  _oldStreambuf[1] = std::clog.rdbuf();
  _oldStreambuf[2] = std::cerr.rdbuf();
  std::cout.rdbuf(&_cout);
  std::clog.rdbuf(&_clog);
  std::cerr.rdbuf(&_cerr);
  //cout
  connect(&_cout,SIGNAL(overflowed()),this,SLOT(cout_handle()));
  connect(&_cout,SIGNAL(synchronized()),this,SLOT(cout_handle()));
  connect(&_cout,SIGNAL(xsputned()),this,SLOT(cout_handle()));
  //clog
  connect(&_clog,SIGNAL(overflowed()),this,SLOT(clog_handle()));
  connect(&_clog,SIGNAL(synchronized()),this,SLOT(clog_handle()));
  connect(&_clog,SIGNAL(xsputned()),this,SLOT(clog_handle()));
  //cerr
  connect(&_cerr,SIGNAL(overflowed()),this,SLOT(cerr_handle()));
  connect(&_cerr,SIGNAL(synchronized()),this,SLOT(cerr_handle()));
  connect(&_cerr,SIGNAL(xsputned()),this,SLOT(cerr_handle()));
}

Logger::~Logger()
{
  std::cout.rdbuf(_oldStreambuf[0]);
  std::clog.rdbuf(_oldStreambuf[1]);
  std::cerr.rdbuf(_oldStreambuf[2]);
  delete ui;
}

void Logger::cout_handle()
{
  QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());
  auto signal = metaMethod.name();
  QString text = _cout.readAll();
  if (signal == "synchronized")
    {
      if (_synchro[0] == 1) return; // We are already synchronized
      _synchro[0] = 1;
      text += "\n";
    }
  else
    {
      _synchro[0] = 0;
    }
  Logger::finishSynchro(text);
}

void Logger::clog_handle()
{
  QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());
  auto signal = metaMethod.name();
  QString text = _clog.readAll();
  if (signal == "synchronized")
    {
      if (_synchro[1] == 1) return; // We are already synchronized
      _synchro[1] = 1;
      text += "\n";
    }
  else
    {
      _synchro[1] = 0;
    }
  Logger::finishSynchro(text);
}

void Logger::cerr_handle()
{
  QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());
  auto signal = metaMethod.name();
  QString text = _cerr.readAll();
  if (signal == "synchronized")
    {
      if (_synchro[2] == 2) return; // We are already synchronized
      else if (_synchro[2] == 1) text += "\n";
      ++_synchro[2];
    }
  else
    {
      _synchro[2] = 0;
    }
  Logger::finishSynchro(text);
}

void Logger::on_more_toggled(bool checked)
{
  checked ? ui->logger->show() : ui->logger->hide();
  if (checked)
    {
      ui->logger->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
      ui->console->setText(tr("Console"));
      ui->info->hide();
      ui->warning->hide();
      ui->error->hide();
    }
}

void Logger::on_debug_toggled(bool checked)
{
  if (checked) utils::dumpConfig(std::clog);
  Exception::setVerbosity(checked?VDEBUG:5);
  emit(debugMode(checked));
}

void Logger::on_trash_clicked()
{
  ui->console->setText(tr("Console"));
  ui->logger->clear();
  ui->trash->setIcon(QIcon(":/logger/icons/trash.svg"));
}

void Logger::on_copy_clicked()
{
   QApplication::clipboard()->setText(ui->logger->toPlainText());
   emit(copied(tr("Content copied to clipboard"),2000));
}

void Logger::mousePressEvent(QMouseEvent *mouseEvent)
{
  if (mouseEvent->button() == Qt::LeftButton)
    ui->more->toggle();
}

void Logger::finishSynchro(const QString &text)
{
  ui->logger->insertPlainText(text);
  if (ui->autoscroll->isChecked())
    ui->logger->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
  ui->trash->setIcon(QIcon(":/logger/icons/trash-2.svg"));
  if (ui->more->isChecked()) return;
  if (text.contains("Warning")) ui->warning->show();
  if (text.contains("Error")) ui->error->show();
  if (text.contains("Comment")) ui->info->show();
  ui->console->setText(tr("Console*"));
}
