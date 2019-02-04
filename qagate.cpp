#include "qagate.h"
#include "ui_qagate.h"

qAgate::qAgate(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::qAgate)
{
  ui->setupUi(this);
}

qAgate::~qAgate()
{
  delete ui;
}
