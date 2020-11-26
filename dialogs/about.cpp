#include "about.h"
#include "ui_about.h"
#include "agate.h"
#include "base/utils.hpp"
#include "QDebug"

About::About(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::About)
{
  ui->setupUi(this);
  ui->credits->hide();
  ui->licence->hide();
  QIcon logo(":/qagate.svg");
  ui->logo->setPixmap(logo.pixmap(QSize(100,100)));
  ui->version->setText(ui->version->text().replace("QAGATE_VERSION",QAGATE_VERSION));
  QString about = ui->aboutText->text().replace("QT_VERSION",qVersion());
  about = about.replace("ABIOUT_VERSION",QString::fromStdString(utils::agateVersion()));
#if defined(HAVE_SPGLIB) && defined(HAVE_SPGLIB_VERSION)
  about = about.replace("SPGLIB_VERSION",QString::fromStdString(utils::spglibVersion()));
#else
  about = about.replace("SPGLIB_VERSION",tr("no"));
#endif
  ui->aboutText->setText(about);
  ui->buttonBox->button(QDialogButtonBox::Close)->setText(tr("&Close"));
}

About::~About()
{
  delete ui;
}

void About::on_buttonBox_rejected()
{
  this->close();
}

void About::on_aboutButton_clicked()
{
  ui->about->show();
  ui->aboutButton->setChecked(true);
  ui->credits->hide();
  ui->creditButton->setChecked(false);
  ui->licence->hide();
  ui->licenceButton->setChecked(false);
}

void About::on_creditButton_clicked()
{
  ui->about->hide();
  ui->aboutButton->setChecked(false);
  ui->credits->show();
  ui->creditButton->setChecked(true);
  ui->licence->hide();
  ui->licenceButton->setChecked(false);
}

void About::on_licenceButton_clicked()
{
  ui->about->hide();
  ui->aboutButton->setChecked(false);
  ui->credits->hide();
  ui->creditButton->setChecked(false);
  ui->licence->show();
  ui->licenceButton->setChecked(true);
}
