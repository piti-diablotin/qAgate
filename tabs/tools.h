#ifndef TOOLS_H
#define TOOLS_H

#include <QWidget>
#include "abstracttab.h"
#include "qconducti/qconducti.h"
#include "qdispersion/qdispersion.h"
#include "qtdep/qtdep.h"
#include "qdos/qdos.h"

namespace Ui {
  class Tools;
}

class Tools : public AbstractTab
{
  Q_OBJECT

public:
  explicit Tools(QWidget *parent = 0);
  ~Tools();

protected:
  virtual void updateStatus(View* view);

private slots:
  void on_conducti_clicked();

  void on_dispersion_clicked();

  void on_tdep_clicked();

  void on_dos_clicked();

private:
  Ui::Tools *ui;
  QString _file;
  QConducti _conducti;
  QDispersion _dispersion;
  qTdep _tdep;
  QDos _dos;
};

#endif // TOOLS_H
