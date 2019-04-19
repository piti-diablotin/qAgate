#include "qtdepsupercell.h"
#include "ui_qtdepsupercell.h"
#include <QMessageBox>
#include <QMimeData>
#include "base/utils.hpp"
#include "io/dtset.hpp"
#include <QDebug>

qTdepSupercell::qTdepSupercell(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::qTdepSupercell),
  _supercell(nullptr)
{
  ui->setupUi(this);
}

qTdepSupercell::~qTdepSupercell()
{
  delete ui;
  if (_supercell!=nullptr)
    delete _supercell;
}

void qTdepSupercell::dragEnterEvent(QDragEnterEvent *dragEnterEvent)
{
  if (dragEnterEvent->mimeData()->hasFormat("text/uri-list"))
    dragEnterEvent->acceptProposedAction();
}

void qTdepSupercell::dropEvent(QDropEvent *dropEvent)
{
  QList<QUrl> urls = dropEvent->mimeData()->urls();
  if (urls.isEmpty())
    return;

  QString fileName = urls.first().toLocalFile();
  if (fileName.isEmpty())
    return;
  this->openFile(fileName);
}

void qTdepSupercell::openFile(const QString &filename)
{
  try
  {
    _supercell = dynamic_cast<HistDataMD*>(HistData::getHist(filename.toStdString(),true));
    if (_supercell==nullptr) throw EXCEPTION(tr("Unable to have an MD trajectory").toStdString(),ERRDIV);
    this->histToView();
    size_t pos = _supercell->filename().find_last_of("/\\");
    qDebug() << pos;
    emit(openedFile(QString::fromStdString(_supercell->filename().substr(pos+1))));
  }
  catch(Exception &e)
  {
    QMessageBox::critical(this,tr("Error"),QString::fromStdString(e.fullWhat()));
  }
}

void qTdepSupercell::histToView()
{
  if (_supercell == nullptr) return;
  int ntime = _supercell->ntime();
  ui->tbegin->setRange(0,ntime-1);
  ui->tend->setRange(0,ntime-1);
  ui->tend->setValue(ntime-1);
  ui->trajectory->setTitle(tr("Trajectory (%n step(s))","",ntime));
  this->updateTemperature();
  Dtset first(*_supercell,0);
  first.standardizeCell(true,0.01);
  this->updateMultiplicity(first);
  emit(supercellChanged(first));
}

void qTdepSupercell::updateTemperature()
{
  std::vector<double> temperature(ui->tend->value()-ui->tbegin->value()+1);
  unsigned litime = 0;
  for (int itime = ui->tbegin->value(); itime < ui->tend->value()+1; ++itime)
    {
      temperature[litime++] = _supercell->getTemperature(itime);
    }
  double meanT = utils::mean(temperature.begin(),temperature.end());
  ui->temperature->setValue(meanT);
}

void qTdepSupercell::updateMultiplicity(const Dtset &unitcell)
{
  geometry::mat3d rprimUC = unitcell.rprim();
  const double *rprimTmp = _supercell->getRprimd(0);
  geometry::mat3d rprimSC;
  for (unsigned i = 0; i < 9; ++i )  rprimSC[i] = rprimTmp[i];
  geometry::mat3d passage = geometry::invert(rprimUC);
  auto multiplicity = geometry::operator*(rprimSC,passage);
  ui->xx->setText(QString::number(multiplicity[0]));
  ui->yx->setText(QString::number(multiplicity[1]));
  ui->zx->setText(QString::number(multiplicity[2]));
  ui->xy->setText(QString::number(multiplicity[3]));
  ui->yy->setText(QString::number(multiplicity[4]));
  ui->zy->setText(QString::number(multiplicity[5]));
  ui->xz->setText(QString::number(multiplicity[6]));
  ui->yz->setText(QString::number(multiplicity[7]));
  ui->zz->setText(QString::number(multiplicity[8]));
}
