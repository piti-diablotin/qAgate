#include "qdos.h"
#include <QApplication>
#include "base/mendeleev.hpp"

Agate::mendeleev Agate::Mendeleev;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Exception::setVerbosity(5);
  QString locale = QLocale::system().name().section('_', 0, 0);
  QTranslator translator;
  translator.load(QString(":/translations/qdos_")+ locale);
  a.installTranslator(&translator);
  QDos w;
  w.show();

  return a.exec();
}
