#include "qdispersion.h"
#include "ui_qdispersion.h"

QDispersion::QDispersion(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QDispersion)
{
  ui->setupUi(this);
}

QDispersion::~QDispersion()
{
  delete ui;
}
