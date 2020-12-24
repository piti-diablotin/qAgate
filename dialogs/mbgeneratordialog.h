#ifndef MBGENERATORDIALOG_H
#define MBGENERATORDIALOG_H

#include <QDialog>

namespace Ui {
  class MBGeneratorDialog;
}

class MBGeneratorDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit MBGeneratorDialog(QWidget *parent = nullptr);
    ~MBGeneratorDialog();
    QString seedType() const;
    int seed() const;
    QString distribution() const;
    QString statistics() const;

  private slots:
    void on_seedType_currentIndexChanged(int index);

    void on_seed_editingFinished();

  private:
    Ui::MBGeneratorDialog *ui;
};

#endif // MBGENERATORDIALOG_H
