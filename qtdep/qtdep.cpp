#include "qtdep.h"
#include "ui_qtdep.h"

qTdep::qTdep(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::qTdep)
{
    ui->setupUi(this);
}

qTdep::~qTdep()
{
    delete ui;
}
