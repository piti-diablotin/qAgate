#ifndef QTDEPSUPERCELL_H
#define QTDEPSUPERCELL_H

#include <QWidget>
#include <QDropEvent>
#include <QDragEnterEvent>
#include "hist/histdatamd.hpp"
#include "io/dtset.hpp"

namespace Ui {
  class qTdepSupercell;
}

class qTdepSupercell : public QWidget
{
  Q_OBJECT

public:
  explicit qTdepSupercell(QWidget *parent = 0);
  ~qTdepSupercell();
  virtual void dragEnterEvent(QDragEnterEvent *dragEnterEvent);
  virtual void dropEvent(QDropEvent *dropEvent);

  void openFile(const QString &filename);
  void histToView();


private slots:
  void updateTemperature();
  void updateMultiplicity(const Dtset& unitcell);

signals:
  void openedFile(const QString &);
  void supercellChanged(const Dtset& unitcell);
  void rcutHint(double rcut);

private:
  Ui::qTdepSupercell *ui;
  HistDataMD *_supercell;
};

#endif // QTDEPSUPERCELL_H
