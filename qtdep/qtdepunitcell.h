#ifndef QTDEPUNITCELL_H
#define QTDEPUNITCELL_H

#include <QWidget>
#include <QStandardItemModel>
#include <QDropEvent>
#include <QDragEnterEvent>
#include "io/dtset.hpp"
#include "tools/typatitemdelegate.h"
#include "bind/tdep.hpp"

namespace Ui {
  class qTdepUnitcell;
}

class qTdepUnitcell : public QWidget
{
  Q_OBJECT

public:
  explicit qTdepUnitcell(QWidget *parent = 0);
  ~qTdepUnitcell();

  virtual void dragEnterEvent(QDragEnterEvent *dragEnterEvent);
  virtual void dropEvent(QDropEvent *dropEvent);

  void openFile(const QString &filename);
  Dtset viewToDtset();
  void setTdep(Tdep &tdep);

private slots:
  void dtsetToView(const Dtset& dtset);
  void updateSpg();

signals:
  void unitcellChanged(const Dtset&);
  void showMessage(const QString);

private:
  Ui::qTdepUnitcell *ui;
  QStandardItemModel _atomModel;
  TypatItemDelegate _typatDelegate;
};

#endif // QTDEPUNITCELL_H
