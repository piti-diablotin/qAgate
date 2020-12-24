#ifndef MULTIBINIT_H
#define MULTIBINIT_H

#include <QWidget>
#include "tabs/abstracttab.h"
#include "dialogs/mbgeneratordialog.h"

namespace Ui {
  class MultibinitTab;
}

class MultibinitTab : public AbstractTab
{
    Q_OBJECT

  public:
    explicit MultibinitTab(QWidget *parent = 0);
    ~MultibinitTab();

protected:
  virtual void updateStatus(View* view);

  private slots:
    void on_instable_currentIndexChanged(int index);

    void on_generate_clicked();

    void on_randomHist_clicked(bool checked);

    void on_trajHist_clicked(bool checked);

    void on_browse_clicked();

    void on_pumpButton_clicked();

    void on_generatorSettings_clicked();

  private:
    Ui::MultibinitTab *ui;
    MBGeneratorDialog _mbGenerator;
};

#endif // MULTIBINIT_H
