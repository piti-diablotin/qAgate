#include "qconducti.h"
#include <QApplication>
#include "base/exception.hpp"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Exception::setVerbosity(5);
  QString locale = QLocale::system().name().section('_', 0, 0);
  QTranslator translator;
  [[maybe_unused]] auto transLoaded = translator.load(QString(":/translations/qconducti_")+ locale);
  a.installTranslator(&translator);
  QConducti w;
  w.show();
  return a.exec();
}
