#include "qtdep.h"
#include <QApplication>
#include <QTranslator>
#include "base/mendeleev.hpp"

Agate::mendeleev Agate::Mendeleev;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Exception::setVerbosity(5);
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString(":/translations/qconducti_")+ locale);
    a.installTranslator(&translator);
    qTdep w;
    w.show();

    return a.exec();
}
