#include "tools.h"
#include "ui_tools.h"

Tools::Tools(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::Tools)
{
  ui->setupUi(this);
}

Tools::~Tools()
{
  delete ui;
}

void Tools::updateStatus(View *view)
{
  if (view->getCanvas() != nullptr && _conducti.isHidden())
    _conducti.openFile(QString::fromStdString(view->getCanvas()->info()));
}

void Tools::on_toolButton_clicked()
{
    _conducti.show();
}
