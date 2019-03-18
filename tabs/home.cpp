#include "home.h"
#include "ui_home.h"
#include "dialogs/dumpdialog.h"
#include <QLocale>
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
  _distanceUnit(UnitConverter::bohr)
{
  ui->setupUi(this);
  ui->choiceWhat->setItemData(0,"");
  ui->choiceWhat->setItemData(1,"all");
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
  ui->dumpHist->setEnabled(something);
  ui->dumpXyz->setEnabled(something);
  ui->saveAbinit->setEnabled(something);
  ui->savePoscar->setEnabled(something);
  ui->saveCif->setEnabled(something);
  ui->update->setEnabled(something);

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

  _natom = something ? view->getCanvas()->histdata()->natom() : 0;
  ui->distanceAtom1->setRange(1,_natom);
  ui->distanceAtom2->setRange(1,_natom);
  ui->angleAtom1->setRange(1,_natom);
  ui->angleAtom2->setRange(1,_natom);
  ui->angleAtom3->setRange(1,_natom);

  if (something)
    {
      _znucl = QVector<int>::fromStdVector(view->getCanvas()->histdata()->znucl());
    }
}

void Home::on_update_clicked()
{
   emit(sendCommand(":update"));
}

void Home::on_dumpHist_clicked()
{
  if (_dumpDialog.exec() != QDialog::Accepted|| _dumpDialog.filename().isEmpty()) return;
   emit(sendCommand(":dumphist "+_dumpDialog.filename()+" step "
                    +QString::number(_dumpDialog.step())));
}

void Home::on_dumpXyz_clicked()
{
  if (_dumpDialog.exec()!=QDialog::Accepted || _dumpDialog.filename().isEmpty()) return;
   emit(sendCommand(":dumpxyz "+_dumpDialog.filename()+" step "
                    +QString::number(_dumpDialog.step())));
  _writeDialog.setDirectory(_dumpDialog.directory());
}

void Home::on_saveAbinit_clicked()
{
  if (_writeDialog.exec()!=QDialog::Accepted || _writeDialog.filename().isEmpty()) return;
   emit(sendCommand(":write dtset "+QString::number(_writeDialog.precision())
                    +" "+_writeDialog.filename()+" "+_writeDialog.option()));
  _dumpDialog.setDirectory(_writeDialog.directory());
}

void Home::on_savePoscar_clicked()
{
  if (_writeDialog.exec()!=QDialog::Accepted || _writeDialog.filename().isEmpty()) return;
   emit(sendCommand(":write POSCAR "+QString::number(_writeDialog.precision())
                    +" "+_writeDialog.filename()+" "+_writeDialog.option()));
  _dumpDialog.setDirectory(_writeDialog.directory());
}

void Home::on_saveCif_clicked()
{
  if (_writeDialog.exec()!=QDialog::Accepted || _writeDialog.filename().isEmpty()) return;
   emit(sendCommand(":write cif "+QString::number(_writeDialog.precision())
                    +" "+_writeDialog.filename()+" "+_writeDialog.option()));
  _dumpDialog.setDirectory(_writeDialog.directory());
}

void Home::on_load_clicked()
{
  auto fileNames = QFileDialog::getOpenFileNames(this,"Open File",_writeDialog.directory(),"");
  if ( !fileNames.empty() )
    {
      for ( auto file = fileNames.begin() ; file != fileNames.end() ; ++file )
        emit(sendCommand(":load "+*file));
      auto dir = fileNames.begin()->section("/",0,-2);
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