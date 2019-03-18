#include "tools.h"
#include "ui_tools.h"

Tools::Tools(QWidget *parent) :
  AbstractTab(parent),
  _file(),
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
  if (view->getCanvas() != nullptr )
    _file = QString::fromStdString(view->getCanvas()->info();
}

void Tools::on_toolButton_clicked()
{
  if (_conducti.isHidden())
    _conducti.openFile(_file_);
  _conducti.show();
}
