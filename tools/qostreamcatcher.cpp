#include "qostreamcatcher.h"
#include<QDebug>

QOstreamCatcher::QOstreamCatcher(QWidget* parent): QWidget(parent), streambuf(),
  buffer(),
  _data(),
  _dataString()
{
    _data.setString(&_dataString);
    setp( buffer, buffer + bufferSize - 1 );
}

int QOstreamCatcher::sync()
{
  emit synchronized();
  return streambuf::sync();
}

basic_streambuf<char>::int_type QOstreamCatcher::overflow(basic_streambuf<char>::int_type v)
{
  if ( pbase() == NULL ) {
      // save one char for next overflow:
      setp( buffer, buffer + bufferSize - 1 );
      if ( v != EOF ) {
          v = sputc( v );
        } else {
          v = 0;
        }
    } else {
      char* end = pptr();
      if ( v != EOF ) {
          *end ++ = v;
        }
      if ( false ) {
          v = EOF;
        } else if ( v == EOF ) {
          v = 0;
        }
      setp( buffer, buffer + bufferSize - 1 );
    }
  emit overflowed();
  return v;
}

streamsize QOstreamCatcher::xsputn(const char* p, streamsize n)
{
  for (int c = 0; c < n; ++c)
    _data << p[c];
  setp( buffer, buffer + bufferSize - 1 );
  emit xsputned();
  return n;
}
