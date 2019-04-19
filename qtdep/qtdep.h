#ifndef QTDEP_H
#define QTDEP_H

#include <QMainWindow>

namespace Ui {
class qTdep;
}

class qTdep : public QMainWindow
{
    Q_OBJECT

public:
    explicit qTdep(QWidget *parent = 0);
    ~qTdep();

private slots:
  void updateFilename(const QString &filename);

private:
    Ui::qTdep *ui;
};

#endif // QTDEP_H
