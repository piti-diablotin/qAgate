#include "phononsmode.h"
#include "ui_phononsmode.h"

PhononsMode::PhononsMode(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::PhononsMode)
{
  ui->setupUi(this);
}

PhononsMode::~PhononsMode()
{
  delete ui;
}

void PhononsMode::updateStatus(View *view)
{
  (void) view;
}
