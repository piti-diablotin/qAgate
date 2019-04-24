#include "tools.h"
#include "ui_tools.h"

Tools::Tools(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::Tools),
  _file(),
  _conducti(this)
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
    _file = QString::fromStdString(view->getCanvas()->info());
}

void Tools::on_conducti_clicked()
{
  if (_conducti.isHidden() && !_file.isEmpty())
    _conducti.openFile(_file);
  _conducti.show();
  _conducti.activateWindow();
}

void Tools::on_dispersion_clicked()
{
  //if (_dispersion.isHidden() && !_file.isEmpty())
  //  _dispersion.openFile(_file);
  _dispersion.show();
  _dispersion.activateWindow();
}

void Tools::on_toolButton_clicked()
{
  if (_tdep.isHidden() && !_file.isEmpty())
    {
      _tdep.setSupercellWidget();
      _tdep.openFile(_file);
    }
  _tdep.show();
  _tdep.activateWindow();
}
