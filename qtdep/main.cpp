#include "qtdep.h"
#include <QApplication>
#include "base/mendeleev.hpp"

Agate::mendeleev Agate::Mendeleev;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qTdep w;
    w.show();

    return a.exec();
}
