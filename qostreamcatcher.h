#ifndef QOSTREAMCATCHER_H
#define QOSTREAMCATCHER_H

#include <streambuf>
#include <QWidget>
#include <QTextStream>

using namespace std;

class QOstreamCatcher: public QWidget, public streambuf
{
  Q_OBJECT

private:

  static const int bufferSize = 256;
  char buffer[bufferSize];
  QTextStream _data;
  QString _dataString;

public:

  QOstreamCatcher(QWidget* parent);
  QString readAll() {return _data.readAll();}

protected:

  virtual int pubsync();

  virtual int_type overflow(basic_streambuf<char>::int_type v);

  virtual streamsize xsputn(const char* p, streamsize n);

signals:

  void synchronized();
  void overflowed();
  void xsputned();
};

#endif // QOSTREAMCATCHER_H
