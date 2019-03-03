#include "qagate.h"
#include "base/exception.hpp"
#include "base/mendeleev.hpp"
#include <QApplication>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
#include <QSurface>
#endif

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
  qAgate w;
  w.show();

  return a.exec();
}
