#ifndef QTDEP_H
#define QTDEP_H

#include <QMainWindow>
#include <QAbstractButton>

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

  void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::qTdep *ui;
    QString _currentDirectory;
};

#endif // QTDEP_H
