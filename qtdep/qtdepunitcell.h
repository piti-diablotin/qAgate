#ifndef QTDEPUNITCELL_H
#define QTDEPUNITCELL_H

#include <QWidget>

namespace Ui {
class qTdepUnitcell;
}

class qTdepUnitcell : public QWidget
{
    Q_OBJECT

public:
    explicit qTdepUnitcell(QWidget *parent = 0);
    ~qTdepUnitcell();

private:
    Ui::qTdepUnitcell *ui;
};

#endif // QTDEPUNITCELL_H
