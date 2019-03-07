#include "home.h"
#include "ui_home.h"
#include "dialogs/dumpdialog.h"
#include <QLocale>

Home::Home(QWidget *parent) :
  AbstractTab(parent),
  ui(new Ui::Home),
  _dumpDialog(this),
  _writeDialog(this),
  _natom(0),
  _distance(0),
  _distanceUnit(UnitConverter::bohr)
{
  ui->setupUi(this);
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

  ui->distanceCheckBox->setEnabled(something);
  ui->angleCheckBox->setEnabled(something);
  ui->spg->setEnabled(something);

  _natom = something ? view->getCanvas()->histdata()->natom() : 0;
  ui->distanceAtom1->setRange(1,_natom);
  ui->distanceAtom2->setRange(1,_natom);
  ui->angleAtom1->setRange(1,_natom);
  ui->angleAtom2->setRange(1,_natom);
  ui->angleAtom3->setRange(1,_natom);
}

void Home::on_update_clicked()
{
   emit(sendCommand(":update"));
}

void Home::on_dumpHist_clicked()
{
  _dumpDialog.exec();
  if (_dumpDialog.filename().isEmpty()) return;
   emit(sendCommand(":dumphist "+_dumpDialog.filename()+" step "
                    +QString::number(_dumpDialog.step())));
}

void Home::on_dumpXyz_clicked()
{
  _dumpDialog.exec();
  if (_dumpDialog.filename().isEmpty()) return;
   emit(sendCommand(":dumpxyz "+_dumpDialog.filename()+" step "
                    +QString::number(_dumpDialog.step())));
  _writeDialog.setDirectory(_dumpDialog.directory());
}

void Home::on_saveAbinit_clicked()
{
  _writeDialog.exec();
  if (_writeDialog.filename().isEmpty()) return;
   emit(sendCommand(":write dtset "+QString::number(_writeDialog.precision())
                    +" "+_writeDialog.filename()+" "+_writeDialog.option()));
  _dumpDialog.setDirectory(_writeDialog.directory());
}

void Home::on_savePoscar_clicked()
{
  _writeDialog.exec();
  if (_writeDialog.filename().isEmpty()) return;
   emit(sendCommand(":write POSCAR "+QString::number(_writeDialog.precision())
                    +" "+_writeDialog.filename()+" "+_writeDialog.option()));
  _dumpDialog.setDirectory(_writeDialog.directory());
}

void Home::on_saveCif_clicked()
{
  _writeDialog.exec();
  if (_writeDialog.filename().isEmpty()) return;
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

void Home::on_distanceUnit_currentTextChanged(const QString &arg1)
{
  _distanceUnit = UnitConverter::getUnit(arg1.toStdString());
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
   emit(sendCommand(":spg"));
}
