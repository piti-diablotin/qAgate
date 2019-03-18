#include "qagate.h"
#include "base/exception.hpp"
#include "base/mendeleev.hpp"
#include <QApplication>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
#include <QSurface>
#endif
#include <clocale>

Agate::mendeleev Agate::Mendeleev;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
  QSurfaceFormat glf = QSurfaceFormat::defaultFormat();
  glf.setSamples(8);
  QSurfaceFormat::setDefaultFormat(glf);
#else
  QGLFormat glf = QGLFormat::defaultFormat();
  glf.setSampleBuffers(true);
  glf.setSamples(8);
  QGLFormat::setDefaultFormat(glf);
#endif
  Exception::setVerbosity(5);
  QString locale = QLocale::system().name().section('_', 0, 0);
  QTranslator translator;
  translator.load(QString(":/translations/qagate_")+ locale);
  a.installTranslator(&translator);
  // Need to correctly read DDB files and probably numbers elsewhere
  setlocale(LC_NUMERIC,"C");
  //char* loc = setlocale(LC_ALL,"C");
  qAgate w;
  w.show();

  return a.exec();
}
