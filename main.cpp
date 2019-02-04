#include "qagate.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  qAgate w;
  w.show();

  return a.exec();
}
