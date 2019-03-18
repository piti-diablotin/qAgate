#include "qdispersion.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QDispersion w;
  w.show();

  return a.exec();
}
