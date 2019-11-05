#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

namespace Ui {
  class About;
}

class About : public QDialog
{
    Q_OBJECT

  public:
    explicit About(QWidget *parent = 0);
    ~About();

  private slots:
    void on_buttonBox_rejected();

    void on_aboutButton_clicked();

    void on_creditButton_clicked();

    void on_licenceButton_clicked();

  private:
    Ui::About *ui;
};

#endif // ABOUT_H
