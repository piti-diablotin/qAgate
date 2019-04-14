#include "qtdepunitcell.h"
#include "ui_qtdepunitcell.h"

qTdepUnitcell::qTdepUnitcell(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qTdepUnitcell)
{
    ui->setupUi(this);
}

qTdepUnitcell::~qTdepUnitcell()
{
    delete ui;
}
