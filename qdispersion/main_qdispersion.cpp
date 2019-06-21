#include "qdispersion.h"
#include <QApplication>
#include "base/mendeleev.hpp"
#include "base/exception.hpp"
#include <iostream>
#include <QTranslator>

Agate::mendeleev Agate::Mendeleev;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Exception::setVerbosity(5);
  QString locale = QLocale::system().name().section('_', 0, 0);
  QTranslator translator;
  translator.load(QString(":/translations/qdispersion_")+ locale);
  a.installTranslator(&translator);
  QDispersion w;
  std::streambuf* bufstderr = std::cerr.rdbuf();
  std::streambuf* bufstdlog = std::clog.rdbuf();
  std::ofstream fstdlog;
  std::ofstream fstderr;

#ifndef _WIN32
  fstdlog.open("/dev/null",std::ios::out);
  fstderr.open("/dev/null",std::ios::out);
#else
  fstdlog.open("NUL",std::ios::out);
  fstderr.open("NUL",std::ios::out);
#endif
  std::clog.rdbuf(fstdlog.rdbuf());
  std::cerr.rdbuf(fstderr.rdbuf());

  w.show();
  int r = a.exec();

  std::cerr.rdbuf(bufstderr);
  if ( fstderr ) fstderr.close();
  std::clog.rdbuf(bufstdlog);
  if ( fstdlog ) fstdlog.close();

  return r;
}
