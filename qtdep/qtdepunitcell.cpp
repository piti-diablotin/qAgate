#include "qtdepunitcell.h"
#include "ui_qtdepunitcell.h"
#include "io/dtset.hpp"
#include "base/mendeleev.hpp"
#include "tools/doubleitemdelegate.h"
#include <QDoubleValidator>
#include <QMimeData>
#include <QMessageBox>
#include <QComboBox>
#include "hist/histdatadtset.hpp"

qTdepUnitcell::qTdepUnitcell(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::qTdepUnitcell),
  _atomModel(),
  _typatDelegate()
{
  ui->setupUi(this);
  QDoubleValidator *dv = new QDoubleValidator(this);
  ui->acellA->setValidator(dv);
  ui->acellB->setValidator(dv);
  ui->acellC->setValidator(dv);
  ui->ax->setValidator(dv);
  ui->bx->setValidator(dv);
  ui->cx->setValidator(dv);
  ui->ay->setValidator(dv);
  ui->by->setValidator(dv);
  ui->cy->setValidator(dv);
  ui->az->setValidator(dv);
  ui->bz->setValidator(dv);
  ui->cz->setValidator(dv);

  _typatDelegate.setParent(ui->tableView);
  DoubleItemDelegate *doubleDelegate = new DoubleItemDelegate(ui->tableView);
  ui->tableView->setItemDelegateForColumn(0,&_typatDelegate);
  ui->tableView->setItemDelegateForColumn(1,doubleDelegate);
  ui->tableView->setItemDelegateForColumn(2,doubleDelegate);
  ui->tableView->setItemDelegateForColumn(3,doubleDelegate);
  ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->tableView->setModel(&_atomModel);
  ui->tableView->horizontalHeader()->setStretchLastSection(true);
  connect(&_atomModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(updateSpg()));
  connect(ui->acellA,SIGNAL(editingFinished()),this,SLOT(updateSpg()));
  connect(ui->acellB,SIGNAL(editingFinished()),this,SLOT(updateSpg()));
  connect(ui->acellC,SIGNAL(editingFinished()),this,SLOT(updateSpg()));
  connect(ui->ax,SIGNAL(editingFinished()),this,SLOT(updateSpg()));
  connect(ui->ay,SIGNAL(editingFinished()),this,SLOT(updateSpg()));
  connect(ui->ax,SIGNAL(editingFinished()),this,SLOT(updateSpg()));
  connect(ui->bx,SIGNAL(editingFinished()),this,SLOT(updateSpg()));
  connect(ui->by,SIGNAL(editingFinished()),this,SLOT(updateSpg()));
  connect(ui->bz,SIGNAL(editingFinished()),this,SLOT(updateSpg()));
  connect(ui->cx,SIGNAL(editingFinished()),this,SLOT(updateSpg()));
  connect(ui->cy,SIGNAL(editingFinished()),this,SLOT(updateSpg()));
  connect(ui->cz,SIGNAL(editingFinished()),this,SLOT(updateSpg()));
}

qTdepUnitcell::~qTdepUnitcell()
{
  delete ui;
}

void qTdepUnitcell::dragEnterEvent(QDragEnterEvent *dragEnterEvent)
{
  if (dragEnterEvent->mimeData()->hasFormat("text/uri-list"))
    dragEnterEvent->acceptProposedAction();
}

void qTdepUnitcell::dropEvent(QDropEvent *dropEvent)
{
  QList<QUrl> urls = dropEvent->mimeData()->urls();
  if (urls.isEmpty())
    return;

  QString fileName = urls.first().toLocalFile();
  if (fileName.isEmpty())
    return;
  this->openFile(fileName);
}

void qTdepUnitcell::openFile(const QString &filename)
{
  Dtset dtset;
  try
  {
    dtset.readFromFile(filename.toStdString());
    this->dtsetToView(dtset);
    emit(unitcellChanged(dtset));
  }
  catch(Exception &e)
  {
    QMessageBox::critical(this,tr("Error"),QString::fromStdString(e.fullWhat()));
  }
}

void qTdepUnitcell::dtsetToView(const Dtset &dtset)
{
  _atomModel.clear();
  auto typat = dtset.typat();
  auto znucl = dtset.znucl();
  auto xred = dtset.xred();
  for (unsigned iatom = 0 ; iatom < dtset.natom() ; ++iatom)
    {
      QStandardItem *Type = new QStandardItem(QString(Agate::mendeleev::name[znucl[typat[iatom]-1]]));
      QStandardItem *x = new QStandardItem(QString::number(xred[iatom][0],'g',15));
      QStandardItem *y = new QStandardItem(QString::number(xred[iatom][1],'g',15));
      QStandardItem *z = new QStandardItem(QString::number(xred[iatom][2],'g',15));
      QList<QStandardItem*> list;
      list << Type << x << y << z;
      _atomModel.appendRow(list);
    }
  _atomModel.setHeaderData(0,Qt::Horizontal,tr(" Type "));
  _atomModel.setHeaderData(1,Qt::Horizontal,tr("x (red.)"));
  _atomModel.setHeaderData(2,Qt::Horizontal,tr("y (red.)"));
  _atomModel.setHeaderData(3,Qt::Horizontal,tr("z (red.)"));
  _typatDelegate.setTypat(dtset.znucl());
  ui->tableView->resizeColumnToContents(0);
  ui->tableView->resizeColumnToContents(1);
  ui->tableView->resizeColumnToContents(2);
  int c1 = ui->tableView->columnWidth(1);
  int c2 = ui->tableView->columnWidth(2);
  int c3 = ui->tableView->columnWidth(3);
  c3 = (c1+c2+c3)/3;
  ui->tableView->setColumnWidth(1,c3);
  ui->tableView->setColumnWidth(2,c3);
  ui->tableView->setColumnWidth(3,c3);
  auto acell = dtset.acell();
  ui->acellA->setText(QString::number(acell[0],'g',15));
  ui->acellB->setText(QString::number(acell[1],'g',15));
  ui->acellC->setText(QString::number(acell[2],'g',15));
  auto rprim = dtset.rprim();
  ui->ax->setText(QString::number(rprim[0]/acell[0],'g',15));
  ui->bx->setText(QString::number(rprim[1]/acell[1],'g',15));
  ui->cx->setText(QString::number(rprim[2]/acell[2],'g',15));
  ui->ay->setText(QString::number(rprim[3]/acell[0],'g',15));
  ui->by->setText(QString::number(rprim[4]/acell[1],'g',15));
  ui->cy->setText(QString::number(rprim[5]/acell[2],'g',15));
  ui->az->setText(QString::number(rprim[6]/acell[0],'g',15));
  ui->bz->setText(QString::number(rprim[7]/acell[1],'g',15));
  ui->cz->setText(QString::number(rprim[8]/acell[2],'g',15));
  this->updateSpg();
}

Dtset qTdepUnitcell::viewToDtset()
{
  int natom = _atomModel.rowCount();
  if (natom == 0) throw EXCEPTION(tr("Unit cell seems to be empty").toStdString(), ERRDIV);
  QComboBox *cb = qobject_cast<QComboBox *>(_typatDelegate.createEditor(nullptr,QStyleOptionViewItem(),QModelIndex()));
  int ntypat = cb->count();
  QVector<int> znucl(ntypat);
  for (int t = 0; t < ntypat; ++t)
    {
      znucl[t] = Agate::mendeleev::znucl(cb->itemText(t).toStdString());
    }
  QVector<int> typat(natom);
  QVector<QString> xred(3*natom);
  for (int iatom = 0; iatom < natom; ++iatom)
    {
      int t = cb->findText(_atomModel.item(iatom,0)->data(Qt::EditRole).toString())+1;
      if (t==-1) throw EXCEPTION(tr("Unable to fin atom typ").toStdString(), ERRDIV);
      typat[iatom] = t;
      xred[iatom*3+0] = _atomModel.item(iatom,1)->data(Qt::EditRole).toString();
      xred[iatom*3+1] = _atomModel.item(iatom,2)->data(Qt::EditRole).toString();
      xred[iatom*3+2] = _atomModel.item(iatom,3)->data(Qt::EditRole).toString();
    }
  delete cb;
  QString dtsetStr;
  dtsetStr = " natom "+QString::number(natom);
  dtsetStr += " ntypat "+QString::number(ntypat);
  dtsetStr += " znucl ";
  for(auto z : znucl)
    {
      dtsetStr += " " + QString::number(z);
    }
  dtsetStr += " typat ";
  for(auto t : typat)
    {
      dtsetStr += " " + QString::number(t);
    }
  dtsetStr += " acell "+ui->acellA->text() + " " + ui->acellB->text() + " " + ui->acellC->text();
  if ( ui->acellA->text().isEmpty() || ui->acellB->text().isEmpty() || ui->acellC->text().isEmpty() )
    throw EXCEPTION(tr("All lattice scaling factor must be given").toStdString(),ERRDIV);
  if (
      ui->ax->text().isEmpty() ||
      ui->ay->text().isEmpty() ||
      ui->az->text().isEmpty()
      )
    throw EXCEPTION(tr("Please provide the 3 components of vector a").toStdString(), ERRDIV);
  if (
      ui->bx->text().isEmpty() ||
      ui->by->text().isEmpty() ||
      ui->bz->text().isEmpty()
      )
    throw EXCEPTION(tr("Please provide the 3 components of vector b").toStdString(), ERRDIV);
  if (
      ui->cx->text().isEmpty() ||
      ui->cy->text().isEmpty() ||
      ui->cz->text().isEmpty()
      )
    throw EXCEPTION(tr("Please provide the 3 components of vector c").toStdString(), ERRDIV);
  dtsetStr += " rprim "+ui->ax->text() + " ";
  dtsetStr += ui->ay->text() + " ";
  dtsetStr += ui->az->text() + " ";
  dtsetStr += ui->bx->text() + " ";
  dtsetStr += ui->by->text() + " ";
  dtsetStr += ui->bz->text() + " ";
  dtsetStr += ui->cx->text() + " ";
  dtsetStr += ui->cy->text() + " ";
  dtsetStr += ui->cz->text() + " ";
  dtsetStr += " xred ";
  for(auto x : xred)
    {
      dtsetStr += " " + x;
    }
  ConfigParser parser;
  parser.setContent(dtsetStr.toStdString());
  Dtset test;
  test.readConfig(parser);
  return test;
}

void qTdepUnitcell::updateSpg()
{
  Dtset dtset = this->viewToDtset();
  HistDataDtset hist;
  hist.buildFromDtset(dtset);
  int itime = 0;
  double symprec = 1e-3;
  std::string name;
  unsigned spg = hist.getSpgNum(itime,symprec,&name);
  ui->spg->setText(QString::number(spg)+ ": " + QString::fromStdString(name));
}
