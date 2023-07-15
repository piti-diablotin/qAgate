#include "home.h"
#include "ui_home.h"
#include "dialogs/dumpdialog.h"
#include <QLocale>
#include <QFileDialog>
#include "dialogs/supercelldialog.h"
#include "dialogs/movedialog.h"
#include "dialogs/shiftdialog.h"
#include "dialogs/typatdialog.h"

Home::Home(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::Home),
  _dumpDialog(this),
  _writeDialog(this),
  _spgDialog(this),
  _natom(0),
  _distance(0),
  _distanceUnit(UnitConverter::bohr),
  _remoteDialog(this)
{
  ui->setupUi(this);
  ui->choiceWhat->setItemData(0,"");
  ui->choiceWhat->setItemData(1,"all");
  ui->saveAbinit->setDefaultAction(ui->actionAbinit);
  ui->dumpHist->setDefaultAction(ui->actionHIST);
  ui->open->setDefaultAction(ui->actionOpen);
  ui->append->setDefaultAction(ui->actionAppend);
  ui->actionAppend->setDisabled(true);
  ui->cloud->setDefaultAction(ui->actionCloud);
  //connect(ui->actionAbinit,SIGNAL(triggered(bool)),this,SLOT(on_saveAbinit_clicked()));
  //connect(ui->actionHIST,SIGNAL(triggered(bool)),this,SLOT(on_dumpHist_clicked()));
  connect(&_remoteDialog,SIGNAL(sendCommand(QString,bool)),this,SIGNAL(sendCommand(QString,bool)));
  this->addAction(ui->actionAbinit);
  this->addAction(ui->actionHIST);
  this->addAction(ui->actionOpen);
  this->addAction(ui->actionAppend);
  this->addAction(ui->actionCloud);
#ifndef HAVE_SSH
  ui->cloud->setDisabled(true);
#endif
}

Home::~Home()
{
  delete ui;
}

void Home::setDistance(const double distance)
{
  UnitConverter unit = UnitConverter::getFromString(ui->distanceUnit->currentText().toStdString());
  unit.rebase(UnitConverter::bohr);
  _distance = distance;
  ui->distanceResult->setText(QString::number(distance*unit,'g',3));
}

void Home::setAngle(const double angle)
{
  ui->angleResult->setText(QString::number(angle,'g',3)+"°");
}

void Home::getAngleAtoms(int &atom1, int &atom2, int &atom3)
{
  atom1 = ui->angleAtom1->value();
  atom2 = ui->angleAtom2->value();
  atom3 = ui->angleAtom3->value();
}

void Home::getDistanceAtoms(int &atom1, int &atom2)
{
  atom1 = ui->distanceAtom1->value();
  atom2 = ui->distanceAtom2->value();
}

void Home::updateStatus(View *view)
{
  bool something = (view->getCanvas() != nullptr && view->getCanvas()->histdata() != nullptr);
  ui->actionAppend->setEnabled(something);
  ui->dumpHist->setEnabled(something);
  ui->dumpXyz->setEnabled(something);
  ui->saveAbinit->setEnabled(something);
  ui->savePoscar->setEnabled(something);
  ui->saveCif->setEnabled(something);
  ui->update->setEnabled(something);
  ui->dumpDtset->setEnabled(something);

  ui->edit->setEnabled(something);
  ui->explore->setEnabled(something);
  if (something)
  {
    ui->centoid->setEnabled(view->getCanvas()->histdata()->nimage()>1);
  }
  /*
  ui->distanceCheckBox->setEnabled(something);
  ui->angleCheckBox->setEnabled(something);
  ui->spg->setEnabled(something);
  */

  _natom = something ? static_cast<int>(view->getCanvas()->histdata()->natom()) : 0;
  ui->distanceAtom1->setRange(1,_natom);
  ui->distanceAtom2->setRange(1,_natom);
  ui->angleAtom1->setRange(1,_natom);
  ui->angleAtom2->setRange(1,_natom);
  ui->angleAtom3->setRange(1,_natom);

  if (something)
  {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    auto& znucl = view->getCanvas()->histdata()->znucl();
    _znucl = QVector<int>(znucl.begin(),znucl.end());
#else
    _znucl = QVector<int>::fromStdVector(view->getCanvas()->histdata()->znucl());
#endif
  }
}

void Home::on_update_clicked()
{
  emit(sendCommand(":update"));
}

void Home::on_dumpHist_triggered()
{
  if (_dumpDialog.exec() != QDialog::Accepted|| _dumpDialog.filename().isEmpty()) return;
  emit(sendCommand(":dumphist "+_dumpDialog.filename().replace(" ","\\ ")+" step "
                   +QString::number(_dumpDialog.step())));
}

void Home::on_dumpXyz_clicked()
{
  if (_dumpDialog.exec()!=QDialog::Accepted || _dumpDialog.filename().isEmpty()) return;
  emit(sendCommand(":dumpxyz "+_dumpDialog.filename().replace(" ","\\ ")+" step "
                   +QString::number(_dumpDialog.step())));
  _writeDialog.setDirectory(_dumpDialog.directory());
}

void Home::on_saveAbinit_triggered()
{
  if (_writeDialog.exec()!=QDialog::Accepted || _writeDialog.filename().isEmpty()) return;
  emit(sendCommand(QString(":write dtset %0 %1 tolerance=%2")
                   .arg(_writeDialog.filename().replace(" ","\\ ")
                        ,_writeDialog.option()
                        ,QString::number(_writeDialog.precision())
                        )));
  _dumpDialog.setDirectory(_writeDialog.directory());
}

void Home::on_savePoscar_clicked()
{
  if (_writeDialog.exec()!=QDialog::Accepted || _writeDialog.filename().isEmpty()) return;
  emit(sendCommand(QString(":write POSCAR %0 %1 tolerance=%2")
                   .arg(_writeDialog.filename().replace(" ","\\ ")
                        ,_writeDialog.option()
                        ,QString::number(_writeDialog.precision())
                        )));
  _dumpDialog.setDirectory(_writeDialog.directory());
}

void Home::on_saveCif_clicked()
{
  if (_writeDialog.exec()!=QDialog::Accepted || _writeDialog.filename().isEmpty()) return;
  emit(sendCommand(QString(":write cif %0 %1 tolerance=%2")
                   .arg(_writeDialog.filename().replace(" ","\\ ")
                        ,_writeDialog.option()
                        ,QString::number(_writeDialog.precision())
                        )));
  _dumpDialog.setDirectory(_writeDialog.directory());
}

void Home::on_load_clicked()
{
  auto fileNames = QFileDialog::getOpenFileNames(this,tr("Open File"),_writeDialog.directory(),"",nullptr,QFileDialog::DontUseNativeDialog);
  if ( !fileNames.empty() )
  {
    emit(needCommandLine());
    auto file1 = fileNames.first();
    for ( auto file = fileNames.begin() ; file != fileNames.end() ; ++file ) {
      emit(sendCommand(":load "+file->replace(" ","\\ "),false));
    }
    auto dir = file1.section("/",0,-2);
    _writeDialog.setDirectory(dir);
    _dumpDialog.setDirectory(dir);
  }
}

void Home::on_distanceCheckBox_clicked(bool checked)
{
  ui->distanceAtom1->setEnabled(checked);
  ui->distanceAtom1->setRange(1,_natom);
  ui->distanceAtom2->setRange(1,_natom);
  ui->distanceAtom2->setEnabled(checked);
  ui->distanceResult->setEnabled(checked);
  ui->distanceUnit->setEnabled(checked);
  emit(needDistance(checked));
}

void Home::on_angleCheckBox_clicked(bool checked)
{
  ui->angleAtom1->setEnabled(checked);
  ui->angleAtom2->setEnabled(checked);
  ui->angleAtom3->setEnabled(checked);
  ui->angleAtom1->setRange(1,_natom);
  ui->angleAtom2->setRange(1,_natom);
  ui->angleAtom3->setRange(1,_natom);
  ui->angleResult->setEnabled(checked);
  emit(needAngle(checked));
}

void Home::on_distanceUnit_newUnit(UnitConverter::Unit unit)
{
  _distanceUnit = unit;
  double distance = _distance*_distanceUnit;
  ui->distanceResult->setText(QString::number(distance,'g',3));

}

void Home::on_distanceAtom1_valueChanged(int arg1)
{
  (void)  arg1;
  emit(needDistance(true));
}

void Home::on_distanceAtom2_valueChanged(int arg1)
{
  (void)  arg1;
  emit(needDistance(true));
}

void Home::on_angleAtom1_valueChanged(int arg1)
{
  (void)  arg1;
  emit(needAngle(true));
}

void Home::on_angleAtom2_valueChanged(int arg1)
{
  (void)  arg1;
  emit(needAngle(true));
}

void Home::on_angleAtom3_valueChanged(int arg1)
{
  (void)  arg1;
  emit(needAngle(true));
}

void Home::on_spg_clicked()
{
  if (_spgDialog.exec() != QDialog::Accepted) return;
  double precision = _spgDialog.precision();
  if (precision < 0) return;
  emit(sendCommand(":spg "+QString::number(precision,'g',8)));
}

void Home::on_periodic_clicked()
{
  emit(sendCommand(":periodic 1 "+ui->choiceWhat->currentData().toString()));
}

void Home::on_deperiodic_clicked()
{
  emit(sendCommand(":periodic 0 "+ui->choiceWhat->currentData().toString()));
}

void Home::on_average_clicked()
{
  emit(sendCommand(":average"));
}

void Home::on_centoid_clicked()
{
  emit(sendCommand(":centroid"));
}

void Home::on_supercell_clicked()
{
  SupercellDialog dialog(this);
  if (dialog.exec() != QDialog::Accepted) return;
  QVector<int> mat = dialog.matrix();
  if (!mat.isEmpty())
  {
    emit(sendCommand(":supercell "+QString::number(mat[0])
        + " " + QString::number(mat[4])
        + " " + QString::number(mat[8])));
  }
}

void Home::on_move_clicked()
{
  MoveDialog dialog(this);
  dialog.configure(_natom);
  if (dialog.exec() != QDialog::Accepted) return;
  int iatom;
  double x, y ,z;
  dialog.result(iatom,x,y,z);
  if (iatom==0) return;
  emit(sendCommand(":move "+QString::number(iatom)
                   +" " + QString::number(x)
                   +" " + QString::number(y)
                   +" " + QString::number(z)
                   ));
}

void Home::on_shift_clicked()
{
  ShiftDialog dialog(this);
  if (dialog.exec() != QDialog::Accepted) return;
  double x, y ,z;
  dialog.result(x,y,z);
  if (std::abs(x)<1e-6 && std::abs(y)<1e-8 && std::abs(z)<1e-8) return;
  emit(sendCommand(":shift "
                   + QString::number(x)
                   +" " + QString::number(y)
                   +" " + QString::number(z)
                   +ui->choiceWhat->currentData().toString()
                   ));
}

void Home::on_typat_clicked()
{
  TypatDialog dialog(this);
  dialog.configure(_natom,_znucl);
  if (dialog.exec() != QDialog::Accepted) return;
  int iatom, znucl;
  dialog.result(iatom,znucl);
  emit(sendCommand(":typat "+QString::number(iatom)+ " " +QString::number(znucl)));
}

void Home::on_open_triggered()
{
  auto fileNames = QFileDialog::getOpenFileNames(this,"Open File",_currentFolder,INPUT_FILES_FILTER,nullptr,QFileDialog::DontUseNativeDialog);
  if ( !fileNames.empty() )
  {
    QString file1 = fileNames.first();
    if (file1.isEmpty())
      return;
    auto file2 = file1;
    emit(sendCommand(":open "+file2.replace(" ","\\ "),true));
    int pos = file1.lastIndexOf(QRegExp("[/\\\\]"));
    _currentFolder = file1.left(pos+1);

    for ( auto file = fileNames.begin()+1 ; file != fileNames.end() ; ++file )
    {
      if ( !file->isEmpty() )
      {
        emit(sendCommand(":append "+file->replace(" ","\\ "),false));
      }
    }
  }
}

void Home::on_append_triggered()
{
  auto fileNames = QFileDialog::getOpenFileNames(this,"Append File",_currentFolder,INPUT_FILES_FILTER,nullptr,QFileDialog::DontUseNativeDialog);

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
  }

}

void Home::on_actionCloud_triggered()
{
  _remoteDialog.removeAppend(!ui->append->isEnabled());
  _remoteDialog.exec();
}

void Home::on_dumpDtset_clicked()
{
  if (_dumpDialog.exec()!=QDialog::Accepted || _dumpDialog.filename().isEmpty()) return;
  emit(sendCommand(":dumpdtset "+_dumpDialog.filename().replace(" ","\\ ")+" step "
                   +QString::number(_dumpDialog.step())));
  _writeDialog.setDirectory(_dumpDialog.directory());
}
