#include "qagate.h"
#include "base/exception.hpp"
#include "base/mendeleev.hpp"
#include <QApplication>

Agate::mendeleev Agate::Mendeleev;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Exception::setVerbosity(5);
  qAgate w;
  w.show();

  return a.exec();
}
