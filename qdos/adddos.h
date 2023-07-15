#ifndef ADDDOS_H
#define ADDDOS_H

#include <QDialog>
#include "plot/dosdb.hpp"
#include <QStandardItem>

namespace Ui {
  class AddDos;
}

class AddDos : public QDialog
{
  Q_OBJECT

public:
  explicit AddDos(QWidget *parent = 0);
  ~AddDos();

  void setDb(DosDB *db);

  bool hasColor() const;

  QList<QStandardItem*> item();

private slots:
  void on_atom_currentIndexChanged(int index);

  void on_projection_currentTextChanged(const QString &arg1);

  void on_color_clicked();

  void on_magneticLabel_clicked(bool checked);

private:
  Ui::AddDos *ui;
  DosDB *_db;
  bool _hasColor;

};

#endif // ADDDOS_H
