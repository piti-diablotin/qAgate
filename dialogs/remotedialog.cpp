#include "remotedialog.h"
#include "ui_remotedialog.h"
#include "base/ssh.hpp"
#include "base/exception.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QTemporaryDir>
#include <QStandardPaths>
#include "base/utils.hpp"
#include <fstream>
#include <QDebug>
#include <QMetaMethod>

RemoteDialog::RemoteDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::RemoteDialog),
  _intValidator(1,65535,this),
  _textValidator(QRegularExpression("[a-z0-9.-]+"),this),
  _stream(this),
  _filename()
{
  ui->setupUi(this);
  ui->hostname->setValidator(&_textValidator);
  ui->username->setValidator(&_textValidator);
  ui->port->setValidator(&_intValidator);
  ui->protocol->setItemData(0,Ssh::Scp);
  ui->protocol->setItemData(1,Ssh::Sftp);
  QString tmpDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
  ui->local->setText(tmpDir);
  connect(ui->hostname,SIGNAL(textChanged(QString)),this,SLOT(need_to_retest(QString)));
  connect(ui->username,SIGNAL(textChanged(QString)),this,SLOT(need_to_retest(QString)));
  connect(ui->password,SIGNAL(textChanged(QString)),this,SLOT(need_to_retest(QString)));
  connect(ui->port,SIGNAL(textChanged(QString)),this,SLOT(need_to_retest(QString)));
  connect(ui->remote,SIGNAL(textChanged(QString)),this,SLOT(need_to_retest(QString)));
  connect(ui->protocol,SIGNAL(currentIndexChanged(QString)),this,SLOT(need_to_retest(QString)));
}

RemoteDialog::~RemoteDialog()
{
  delete ui;
}

void RemoteDialog::removeAppend(bool remove)
{
  if (remove) ui->append->hide();
  else ui->append->show();
}

void RemoteDialog::on_browse_clicked()
{
  QString dirname = QFileDialog::getExistingDirectory(this,tr("Select directory"),ui->local->text(),QFileDialog::DontUseNativeDialog);
  if (!dirname.isEmpty()) ui->local->setText(dirname);
}

void RemoteDialog::on_test_clicked()
{
  Ssh ssh;
  if (!this->connectSsh(ssh)) return;
  try
  {
    if (ui->remote->text().isEmpty() )
      {
        QMessageBox::warning(this,tr("Warning"),tr("You need to specify a remote file"));
        return;
      }
    Ssh::Protocol proto = (Ssh::Protocol) ui->protocol->currentData().toInt();
    double size = ssh.sizeOfFile(ui->remote->text().toStdString(),proto);
    QString unit = "B";
    if (size>1e9) {size/=1e9;unit="GB";}
    else if (size>1e6) {size/=1e6;unit="MB";}
    else if (size>1e3) {size/=1e3;unit="kB";}
    ui->info->setText(tr("All good! File is ")+QString::number(size,'g',2)+unit);
    ui->get->setEnabled(true);
    ui->progressBar->setEnabled(true);
    ui->progressBar->setValue(0);
  }
  catch (Exception &e)
  {
    ui->info->setText(tr("Unable to get size of file"));
  }
  ssh.disconnect();
}

bool RemoteDialog::connectSsh(Ssh &ssh)
{
  if (ui->hostname->text().isEmpty() )
    {
      QMessageBox::warning(this,tr("Warning"),tr("You need to specify a hostname"));
      return false;
    }
  ssh.setHostname(ui->hostname->text().toStdString());
  ssh.setUsername(ui->username->text().toStdString());
  ssh.setPassword(ui->password->text().toStdString());
  int port = ui->port->text().toInt();
  if (port>0) ssh.setPort(port);
  ui->info->setText(tr("Connecting"));
  try
  {
    ssh.connect();
  }
  catch (Exception &e)
  {
    ui->info->setText(tr("Unable to contact host"));
    return false;
  }
  try
  {
    std::string message;
    bool verif = ssh.verifyHost(message);
    if (!verif)
      {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Verify Host", QString::fromStdString(message)+
                                      "\nAre you sure you want to continue connecting?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            ssh.validateHost();
          } else {
            ui->info->setText(tr("Aborted"));
            return false;
          }
      }
    ssh.authenticate();
    ui->info->setText(tr("Access guaranted"));
  }
  catch (Exception &e)
  {
    ui->info->setText(QString::fromStdString(e.what("",true)));
    return false;
  }
  return true;
}

void RemoteDialog::on_get_clicked()
{
  if (ui->remote->text().isEmpty() )
    {
      QMessageBox::warning(this,tr("Warning"),tr("You need to specify a remote file"));
      return;
    }

  QDir dir(ui->local->text());
  dir.mkpath(dir.absolutePath());
  _filename = dir.absolutePath().toStdString()+"/"+*utils::explode(ui->remote->text().toStdString(),'/').rbegin();
  std::ofstream file(_filename,std::ios::out);
  if (!file)
    {
      QMessageBox::critical(this,tr("Error"),tr("Unable to create file ")+QString::fromStdString(_filename));
      return;
    }

  Ssh ssh;
  if (!this->connectSsh(ssh)) return;
  try
  {
    std::streambuf* oldStreambuf;
    oldStreambuf = std::clog.rdbuf();
    std::clog.rdbuf(&_stream);
    connect(&_stream,SIGNAL(synchronized()),this,SLOT(handle_stream()));
    Ssh::Protocol proto = (Ssh::Protocol) ui->protocol->currentData().toInt();
    ssh.getFile(ui->remote->text().toStdString(),file,proto);
    disconnect(&_stream);
    std::clog.rdbuf(oldStreambuf);
    ui->info->setText(tr("Closing file!"));
    file.close();
    ui->info->setText(tr("Done!"));
    ui->get->setDisabled(true);
    ui->open->setEnabled(true);
    ui->append->setEnabled(true);
  }
  catch (Exception &e)
  {
    ui->info->setText(tr("Unable to get file"));
    _filename.clear();
  }
  ssh.disconnect();
}

void RemoteDialog::handle_stream()
{
  QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());
  auto signal = metaMethod.name();
  if (signal != "synchronized") return;
  bool ok;
  QString line = _stream.readAll();
  QRegularExpression reg("^(?<percent>\\d*)% (?<speed>\\d*\\.\\d{2}[kMG]?B/s)");
  QRegularExpressionMatch match = reg.match(line);
  int percent = match.captured("percent").toInt(&ok,10);
  QString speed = match.captured("speed");
  if (ok)
    {
      ui->progressBar->setValue(percent);
      ui->info->setText(speed);
    }
}

void RemoteDialog::on_open_clicked()
{
  if (_filename.empty()) return;
  emit(sendCommand(QString(":open ")+QString::fromStdString(_filename)));
  this->close();
}

void RemoteDialog::on_append_clicked()
{
  if (_filename.empty()) return;
  emit(sendCommand(QString(":append ")+QString::fromStdString(_filename)));
  this->close();
}

void RemoteDialog::need_to_retest(QString text)
{
  (void) text;
  ui->test->setEnabled(true);
  ui->get->setEnabled(false);
  ui->open->setEnabled(false);
  ui->append->setEnabled(false);
}
