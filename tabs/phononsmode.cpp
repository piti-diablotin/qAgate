#include "phononsmode.h"
#include "ui_phononsmode.h"
#include "canvas/canvasphonons.hpp"
#include "base/fraction.hpp"
#include <QDebug>
#include <QVector3D>
#include <QDoubleValidator>
#include "dialogs/analyzedialog.h"


PhononsMode::PhononsMode(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::PhononsMode),
  _qptModel(),
  _modesModel(),
  _currentQptModes(),
  _doNotUpdate(false),
  _amplitudes()
{
  ui->setupUi(this);
  ui->qpts->setModel(&_qptModel);
  ui->modes->setModel(&_modesModel);
  ui->amplitudeIndiv->setValidator(new QDoubleValidator);
  ui->unit->setCurrentIndex(4);
  connect(&_qptModel,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(addRemoveQpt(QModelIndex,QModelIndex,QVector<int>)));
  connect(&_modesModel,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(addRemoveMode(QModelIndex,QModelIndex,QVector<int>)));
  if (_plot == nullptr )
  {
    _plot = new PlotWindow(this);
  }
}

PhononsMode::~PhononsMode()
{
  delete ui;
}

void PhononsMode::updateStatus(View *view)
{
  _autoUpdate = true;
  const CanvasPhonons* canvas = dynamic_cast<const CanvasPhonons*>(view->canvas());
  this->setEnabled(canvas!=nullptr);
  if (canvas!=nullptr)
  {
    ui->amplitude->setValue(canvas->getAmplitudeDisplacement());
    const DispDB& disp = canvas->getDisplacements();
    const auto qpts = disp.getQpts();
    auto condensed = canvas->getCondensedModes();
    _qptModel.clear();
    ui->qptSelected->setText("");

    for (auto& qpt : qpts)
    {
      QString name = QString::fromStdString(Fraction(qpt[0]).toString())+ " "
          + QString::fromStdString(Fraction(qpt[1]).toString())+ " "
          + QString::fromStdString(Fraction(qpt[2]).toString());

      QStandardItem* itemQpt = new QStandardItem(name);

      QVector3D Qqpt(qpt[0],qpt[1],qpt[2]);
      itemQpt->setData(QVariant::fromValue<QVector3D>(Qqpt),Qt::UserRole);

      itemQpt->setData(Qt::Unchecked,Qt::CheckStateRole);
      itemQpt->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);

      for ( auto& pair : condensed)
      {
        auto q2 = pair.first;
        if (q2==qpt) {
          itemQpt->setData(Qt::Checked,Qt::CheckStateRole);
          break;
        }
      }
      _qptModel.appendRow(itemQpt);
    }
    _qptModel.sort(0,Qt::AscendingOrder);
    if (_qptModel.itemFromIndex(ui->qpts->currentIndex()))
      this->partialUpdate(canvas);
  }
  _autoUpdate = false;
}

void PhononsMode::partialUpdate(const CanvasPhonons *canvas)
{
  if (_doNotUpdate) {
    _doNotUpdate = false;
    return;
  }
  ui->all->setEnabled(true);
  QModelIndex index = ui->qpts->currentIndex();
  QVector3D Qqpt = index.data(Qt::UserRole).value<QVector3D>();
  geometry::vec3d qpt = {Qqpt[0],Qqpt[1],Qqpt[2]};
  UnitConverter unit = ui->unit->currentUnit();
  unit.rebase(UnitConverter::Ha);
  if ( canvas != nullptr )
  {
    const DispDB& disp = canvas->getDisplacements();
    auto condensed = canvas->getCondensedModes();
    _modesModel.clear();
    int dq = disp.getQpt(qpt);
    auto qptItem = _qptModel.itemFromIndex(ui->qpts->currentIndex());
    auto flags = (qptItem->data(Qt::CheckStateRole).toBool()?Qt::ItemIsUserCheckable:Qt::NoItemFlags)|Qt::ItemIsEnabled;
    for ( int i = 0; i < 3*disp.natom() ;++i)
    {
      QStandardItem* it = new QStandardItem(QString("%0 "+QString(QChar(0xC9,0x03))+"=%1").arg(i+1).arg(disp.getEnergyMode(dq,i)*unit));

      it->setFlags(flags);
      if (flags!=Qt::ItemIsEnabled)
        it->setData(Qt::Unchecked,Qt::CheckStateRole);
      it->setData(disp.getEnergyMode(dq,i),Qt::UserRole);
      _modesModel.appendRow(it);
    }
    if (qptItem)
    {
      int allChecked = 0;
      for ( auto& pair : condensed)
      {
        auto q2 = pair.first;
        allChecked = 0;
        if (q2==qpt)
        {
          for( auto& qmode : pair.second )
          {
            int imode = qmode.imode;
            _amplitudes[imode] = qmode.amplitude;
            auto child = _modesModel.item(imode);
            if (child)
            {
              ++allChecked;
              child->setData(Qt::Checked,Qt::CheckStateRole);
            }
          }
          break;
        }
      }
      ui->all->setChecked(allChecked==_modesModel.rowCount());
    }
  }
}

void PhononsMode::on_qpts_activated(const QModelIndex &index)
{
  ui->qptSelected->setText(index.data().toString());
  _autoUpdate = true;
  if (index.data(Qt::CheckStateRole).toBool() && index != _currentQptModes)
    emit(sendCommand(QString(":qpt ")+index.data().toString(),false));
  _currentQptModes = index;
  emit(needPartialUpdate());
  ui->amplitudeIndiv->setDisabled(true);
  ui->amplitudeIndivLabel->setDisabled(true);
  _autoUpdate = false;
}

void PhononsMode::on_unit_currentIndexChanged(int index)
{
  (void) index;
  UnitConverter unit = ui->unit->currentUnit();
  unit.rebase(UnitConverter::Ha);
  _autoUpdate = true;
  for ( int i = 0 ; i < _modesModel.rowCount() ; ++i )
  {
    auto item = _modesModel.item(i);
    item->setData(QString("%0 "+QString(QChar(0xC9,0x03))+"=%1").arg(i+1).arg(item->data(Qt::UserRole).toDouble()*unit),Qt::DisplayRole);
  }
  _autoUpdate = false;
}

void PhononsMode::on_modes_activated(const QModelIndex &index)
{
  auto item = _modesModel.itemFromIndex(index);
  if (item)
  {
    if (item->data(Qt::CheckStateRole).toBool())
    {
      ui->amplitudeIndiv->setEnabled(true);
      ui->amplitudeIndivLabel->setEnabled(true);
      ui->amplitudeIndiv->setText(QString::number(_amplitudes[index.row()]));
    }
    else
    {
      ui->amplitudeIndiv->setDisabled(true);
      ui->amplitudeIndivLabel->setDisabled(true);
    }
  }
}

void PhononsMode::addRemoveQpt(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
  (void) bottomRight;
  (void) roles;
  if (_autoUpdate) return;
  auto qpt = topLeft.data(Qt::DisplayRole).toString();
  if (topLeft.data(Qt::CheckStateRole).toBool()) {
    emit(sendCommand(":add "+qpt));
    ui->qpts->setCurrentIndex(topLeft);
    _currentQptModes = topLeft;
    ui->qptSelected->setText(topLeft.data().toString());
  }
  else
  {
    sendCommand(":rm "+qpt);
    _doNotUpdate = true;
    if (topLeft.row() == _currentQptModes.row()) {
      _autoUpdate = true;
      ui->all->setChecked(false);
      ui->all->setEnabled(false);
      for ( int i = 0 ; i < _modesModel.rowCount() ; ++i )
      {
        auto item = _modesModel.item(i);
        item->setData(Qt::Unchecked,Qt::CheckStateRole);
      }
      _autoUpdate = false;
      _modesModel.clear();
    }
  }
}

void PhononsMode::addRemoveMode(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
  (void) bottomRight;
  (void) roles;
  if (_autoUpdate) return;
  if (topLeft.data(Qt::CheckStateRole).toBool())
  {
    emit(sendCommand(QString(":madd %1").arg(topLeft.row()+1)));
    _amplitudes[topLeft.row()] = ui->amplitude->value();
    if (ui->modes->currentIndex().row()<0)
      ui->modes->setCurrentIndex(topLeft);
  }
  else {
    sendCommand(QString(":mrm %1").arg(topLeft.row()+1));
    _autoUpdate = true;
    ui->all->setChecked(false);
    _autoUpdate = false;
  }
}

void PhononsMode::on_qpts_clicked(const QModelIndex &index)
{
  this->on_qpts_activated(index);
}

void PhononsMode::on_modes_clicked(const QModelIndex &index)
{
  this->on_modes_activated(index);
  if (ui->amplitudeIndiv->isEnabled())
    ui->amplitudeIndivLabel->setText(tr("Amplitude (%1):").arg(index.row()+1));
}

void PhononsMode::on_all_clicked(bool checked)
{
  if(_autoUpdate) return;
  auto state = Qt::Unchecked;
  if (checked)
  {
    emit(sendCommand(":madd all"));
    state = Qt::Checked;
  }
  else if (_modesModel.rowCount() >0 )
  {
    int nmode = _modesModel.rowCount();
    QString toRm(":mrm ");
    for (int imode = 1; imode <= nmode; ++imode)
      toRm += QString(" %1").arg(imode);
    emit(sendCommand(toRm));
    state = Qt::Unchecked;
  }
  else return;
  _autoUpdate = true;
  for ( int i = 0 ; i < _modesModel.rowCount() ; ++i )
  {
    auto item = _modesModel.item(i);
    item->setData(state,Qt::CheckStateRole);
  }
  _autoUpdate = false;
}

void PhononsMode::on_amplitude_valueChanged(double arg1)
{
  if (_autoUpdate) return;
  emit(sendCommand(QString(":amplitude %1").arg(arg1)));
}

void PhononsMode::on_ntime_valueChanged(int arg1)
{
  emit(sendCommand(QString(":ntime %1").arg(arg1)));
}

void PhononsMode::on_amplitudeIndiv_editingFinished()
{
  emit(sendCommand(QString(":amplitude %1 %2").arg(ui->amplitudeIndiv->text()).arg(ui->modes->currentIndex().row()+1)));
  _amplitudes[ui->modes->currentIndex().row()] = ui->amplitudeIndiv->text().toDouble();
}

void PhononsMode::on_reset_clicked()
{
  _autoUpdate = true;
  for ( int i = 0 ; i < _qptModel.rowCount() ; ++i )
  {
    auto item = _qptModel.item(i);
    item->setData(Qt::Unchecked,Qt::CheckStateRole);
  }
  _autoUpdate = true;
  emit(sendCommand(":reset"));
}

void PhononsMode::on_ddb_clicked()
{
  auto fileName = QFileDialog::getSaveFileName(this,tr("Write DDB to File"),_currentFolder,"",nullptr,QFileDialog::DontUseNativeDialog);
  if ( !fileName.isEmpty() )
  {
    emit(sendCommand(":dumpDDB "+fileName));
    int pos = fileName.lastIndexOf(QRegExp("[/\\\\]"));
    _currentFolder = fileName.left(pos+1);
  }
}

void PhononsMode::on_dynamt_clicked()
{
  QString qpt = ui->qptSelected->text();
  if ( qpt.isEmpty() )
  {
    QMessageBox::information(this,tr("Select a qpt"),tr("You need to select a qpt"));
    return;
  }
  //auto fileName = QFileDialog::getSaveFileName(this,tr("Write D(%1) to File").arg(qpt),_currentFolder,"",nullptr,QFileDialog::DontUseNativeDialog);
  //if ( !fileName.isEmpty() )
  //{
  emit(sendCommand(":qpt "+qpt,false));
  emit(sendCommand(":dynmat "/*+fileName*/,false));
  //  int pos = fileName.lastIndexOf(QRegExp("[/\\\\]"));
  //  _currentFolder = fileName.left(pos+1);
  //}
}

void PhononsMode::on_open_clicked()
{
  auto fileNames = QFileDialog::getOpenFileNames(this,"Open File",_currentFolder,"Abinit (*.in *.out *_OUT.nc *_HIST *_HIST.nc *_DDB *_DEN *_OPT *_PHBST.nc);;VASP (POSCAR);;CIF (*.cif);;XML (*.xml);;XYZ (*.xyz);; YAML(*.yaml);;All (*)",nullptr,QFileDialog::DontUseNativeDialog);
  if ( !fileNames.empty() )
  {
    QString file1 = fileNames.first();
    auto file2 = file1;
    if (file1.isEmpty())
      return;
    emit(sendCommand(":open "+file2.replace(" ","\\ ")));
    int pos = file1.lastIndexOf(QRegExp("[/\\\\]"));
    _currentFolder = file1.left(pos+1);

    for ( auto file = fileNames.begin()+1 ; file != fileNames.end() ; ++file )
    {
      if ( !file->isEmpty() )
      {
        emit(sendCommand(":append "+file->replace(" ","\\ "),false));
      }
    }
    emit(needCommandLine());
  }
}

void PhononsMode::on_append_clicked()
{
  auto fileNames = QFileDialog::getOpenFileNames(this,"Append File",_currentFolder,"Abinit (*.in *.out *_OUT.nc *_HIST *_HIST.nc *_DDB *_DEN *_OPT *_PHBST.nc);;VASP (POSCAR);;CIF (*.cif);;XML (*.xml);;XYZ (*.xyz);; YAML (*.yaml);; All (*)",nullptr,QFileDialog::DontUseNativeDialog);

  bool pop = true;
  if ( !fileNames.empty() )
  {
    for ( auto file = fileNames.begin() ; file != fileNames.end() ; ++file )
    {
      if ( !file->isEmpty() )
      {
        auto file2 = *file;
        emit(sendCommand(":append "+file2.replace(" ","\\ "),pop));
        pop = false;
        int pos = file->lastIndexOf(QRegExp("[/\\\\]"));
        _currentFolder = file->left(pos+1);
      }
    }
    emit(needCommandLine());
  }
}

void PhononsMode::on_analyze_clicked()
{
  AnalyzeDialog dialog;
  dialog.setCurrentFolder(_currentFolder);
  if (dialog.exec())
  {
    QPlot* plot;
    _plot->getPlot(PlotWindow::Right,-1,&plot);
    _plot->show();
    emit(plotChanged(plot));
    QString file = dialog.file();
    QString normalization = dialog.normalization();
    _currentFolder = dialog.currentFolder();
    emit(sendCommand(":analyze "+file.replace(" ","\\ ")+" "+normalization));
  }
}

void PhononsMode::on_qpt_clicked()
{
  auto fileName = QFileDialog::getOpenFileName(this,"Open File",_currentFolder,"Abinit (*.in *.out *_OUT.nc *_HIST *_HIST.nc *_DDB *_DEN *_OPT);;VASP (POSCAR);;CIF (*.cif);;XML (*.xml);;XYZ (*.xyz);; YAML (*.yaml);; All (*)",nullptr,QFileDialog::DontUseNativeDialog);
  if ( !fileName.isEmpty() )
  {
    int pos = fileName.lastIndexOf(QRegExp("[/\\\\]"));
    _currentFolder = fileName.left(pos+1);
    QPlot* plot;
    _plot->getPlot(PlotWindow::Left,-1,&plot);
    _plot->show();
    emit(plotChanged(plot));
    emit(sendCommand(":findqpt "+fileName.replace(" ","\\ ")));
  }
}
