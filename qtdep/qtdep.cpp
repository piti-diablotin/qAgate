#include "qtdep.h"
#include "bind/tdep.hpp"
#include "plot/graph.hpp"
#include "ui_qtdep.h"

qTdep::qTdep(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::qTdep), _currentDirectory(),
      _eigparser(nullptr), _energyUnit(UnitConverter::pcm) {
  ui->setupUi(this);
  connect(ui->supercell, SIGNAL(openedFile(const QString &)), this,
          SLOT(updateFilename(const QString &)));
  connect(ui->supercell, SIGNAL(supercellChanged(Dtset)), ui->unitcell,
          SLOT(dtsetToView(Dtset)));
  connect(ui->unitcell, SIGNAL(unitcellChanged(Dtset)), ui->supercell,
          SLOT(updateMultiplicity(Dtset)));
  connect(ui->supercell, SIGNAL(rcutHint(double)), ui->options,
          SLOT(setRcut(double)));
  connect(ui->options, SIGNAL(energyUnitChanged(UnitConverter::Unit)), this,
          SLOT(setEnergyUnit(UnitConverter::Unit)));
  connect(ui->plot, SIGNAL(mouseMove(QMouseEvent *)), this,
          SLOT(coordStatusBar(QMouseEvent *)));
  ui->buttonBox->button(QDialogButtonBox::Save)->setText(tr("&Save"));
  ui->buttonBox->button(QDialogButtonBox::Apply)->setText(tr("&Apply"));
  ui->buttonBox->button(QDialogButtonBox::Close)->setText(tr("&Close"));
  ui->buttonBox->button(QDialogButtonBox::Open)->setText(tr("&Open"));
}

qTdep::~qTdep() { delete ui; }

void qTdep::openFile(const QString &filename) {
  QWidget *widget = ui->toolBox->currentWidget();
  if (dynamic_cast<qTdepSupercell *>(widget) != nullptr) {
    ui->supercell->openFile(filename);
  } else if (dynamic_cast<qTdepUnitcell *>(widget) != nullptr) {
    ui->unitcell->openFile(filename);
  } else {
    QMessageBox::warning(
        this, tr("Opening file"),
        tr("You need to select Supercell or Unit cell to open a file"),
        QMessageBox::Ok);
  }
  int pos = filename.lastIndexOf(QRegExp("[/\\\\]"));
  _currentDirectory = filename.left(pos + 1);
}

void qTdep::setSupercellWidget() {
  ui->toolBox->setCurrentWidget(ui->supercell);
}

void qTdep::showMessage(const QString string) {
  ui->statusBar->showMessage(string);
}

void qTdep::coordStatusBar(QMouseEvent *event) {
  double x = ui->plot->xAxis->pixelToCoord(event->pos().x());
  double y = ui->plot->yAxis->pixelToCoord(event->pos().y());
  QString info = "kpt=" + QString::number(x) + " E=" + QString::number(y);
  ui->statusBar->showMessage(info);
}

void qTdep::tdep() {
  try {
    Tdep tdep;
    ui->supercell->setTdep(tdep);
    ui->unitcell->setTdep(tdep);
    ui->options->setTdep(tdep);
    ui->statusBar->showMessage(tr("Running TDEP..."));
    this->setCursor(Qt::WaitCursor);
    tdep.tdep();
    ui->statusBar->showMessage(tr("Succeeded"));
    _eigparser.reset(
        EigParser::getEigParser(tdep.outputPrefix() + "phonon-bands.yaml"));
    this->replotEigparser();
  } catch (Exception &e) {
    _eigparser.reset();
    QMessageBox::critical(this, tr("Error"),
                          QString::fromStdString(e.fullWhat()));
    ui->statusBar->showMessage(tr("Failed"));
  }
  this->setCursor(Qt::ArrowCursor);
}

void qTdep::updateFilename(const QString &filename) {
  ui->filename->setText(filename);
  ui->toolBox->setCurrentIndex(1);
}

void qTdep::setEnergyUnit(UnitConverter::Unit unit) {
  _energyUnit = unit;
  this->replotEigparser();
}

void qTdep::replotEigparser() {
  if (_eigparser == nullptr) {
    // QMessageBox::information(this,tr("Missing data"),tr("TDEP did not run
    // properly, so there is no dispersion to plot"));
    return;
  }
  ConfigParser config;
  config.setContent(" eunit " + _energyUnit.str());
  Graph::Config conf;
  conf.save = Graph::NONE;
  Graph::plotBand(*_eigparser.get(), config, ui->plot, conf);
}

void qTdep::on_buttonBox_clicked(QAbstractButton *button) {
  QDialogButtonBox::StandardButton b = ui->buttonBox->standardButton(button);
  if (b == QDialogButtonBox::Save) {
    QString filename = QFileDialog::getSaveFileName(
        this, tr("PDF file"), _currentDirectory, "PDF (*.pdf)", nullptr,
        QFileDialog::DontUseNativeDialog);
    if (!filename.endsWith(".pdf"))
      filename += ".pdf";
    ui->plot->savePdf(filename, 0, 0, QCP::epNoCosmetic);
    int pos = filename.lastIndexOf(QRegExp("[/\\\\]"));
    _currentDirectory = filename.left(pos + 1);
  } else if (b == QDialogButtonBox::Open) {
    QWidget *widget = ui->toolBox->currentWidget();
    if (dynamic_cast<qTdepOptions *>(widget) != nullptr) {
      QMessageBox::warning(
          this, tr("Opening file"),
          tr("You need to select Supercell or Unit cell to open a file"),
          QMessageBox::Ok);
      return;
    }
    QString filename = QFileDialog::getOpenFileName(
        this, tr("Open file"), _currentDirectory, "", nullptr,
        QFileDialog::DontUseNativeDialog);
    if (!filename.isEmpty()) {
      this->openFile(filename);
    }
  } else if (b == QDialogButtonBox::Apply) {
    this->tdep();
  } else if (b == QDialogButtonBox::Close)
    this->close();
}
