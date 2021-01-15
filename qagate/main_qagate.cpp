#ifdef HAVE_CONFIG_H
#include "agate.h"
#undef HAVE_CONFIG_H
#endif

#include "qagate.h"
#include "base/exception.hpp"
#include "base/utils.hpp"
#include <QApplication>
#include <QCoreApplication>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
#include <QSurfaceFormat>
#else
#include <QGLFormat>
#endif
#include <clocale>

#include "io/parser.hpp"
#include <csignal>
#include "window/window.hpp"

#include "tools/versionchecker.h"

/**
 * Simple function to display the name and version of the package and what window manager we use.
 */
void Version();

/**
 * Handle signals
 * @param para signal received
 */
void handle_signal (int para);

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QCoreApplication::setApplicationName("qAgate");
  QCoreApplication::setOrganizationName("qAgate");
  QCoreApplication::setOrganizationDomain("qAgate");
  QCoreApplication::setApplicationVersion(QAGATE_VERSION);

  Parser parser(argc,argv);
  parser.setOption("config",'c',"","Configuration file to configure the animation.");
  parser.setOption("version",'v',"Print the version number");
  parser.setOption("help",'h',"Print this message");

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

  int rvalue;
  try
  {
    parser.parse();
    if ( parser.getOption<bool>("version") )
      {
        throw EXCEPTION("",10);
      }
    if ( parser.getOption<bool>("help") )
      {
        throw EXCEPTION("",0);
      }

    signal(SIGABRT,handle_signal);
    signal(SIGFPE,handle_signal);
    signal(SIGILL,handle_signal);
    signal(SIGINT,handle_signal);
    signal(SIGSEGV,handle_signal);
    signal(SIGTERM,handle_signal);
#ifndef _WIN32
    signal(SIGKILL,handle_signal);
    signal(SIGQUIT,handle_signal);
#endif

    qAgate w;
    //w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    std::string config;
    if ( parser.isSetOption("config") )
      {
        config = parser.getOption<std::string>("config");
        w.setParameters(config);
      }

    w.initInput(argc-1,(const char**) argv+1);
  VersionChecker checker;
  checker.checkAgate();
  checker.checkqAgate();
    w.show();
    rvalue = a.exec();
  }
  catch ( Exception& e )
  {
    rvalue = e.getReturnValue();
    if ( rvalue == ERRDIV )
      {
        std::cerr << e.fullWhat() << std::endl;
        if ( rvalue == Parser::ERARG || rvalue == Parser::EROPT ) {
            std::cerr << parser;
            rvalue = 0;
          }
#if defined(WIN32) || defined(_WIN32)
        system("Pause");
#endif
      }
    else if ( rvalue == ERRWAR || rvalue == ERRCOM )
      {
        std::clog << e.fullWhat() << std::endl;
#if defined(WIN32) || defined(_WIN32)
        system("Pause");
#endif
      }
    else if ( rvalue == 10 ) // ask for version number
      {
        Version();
        rvalue = 0;
      }
    else
      {
        std::cout << parser;
        Window::help();
      }
  }

  return rvalue;
}

void handle_signal (int para)
{
  switch(para)
    {
    case SIGABRT :
      std::cerr << "Abord signal received." << std::endl;
      break;
    case SIGFPE :
      std::cerr << "Floating point exception." << std::endl;
      break;
    case SIGILL :
      std::cerr << "Illegal instruction exception." << std::endl;
      break;
    case SIGSEGV :
      std::cerr << "Segmentation fault occured." << std::endl;
      break;
    case SIGTERM :
    case SIGINT :
#ifndef _WIN32
    case SIGQUIT :
    case SIGKILL :
#endif
      std::cerr << "Killing process." << std::endl;
      break;
    default :
      std::cerr << "Unknown signal received." << std::endl;
      break;
    }
  std::cerr << "Window has been asked to close." << std::endl;
  exit(-1);
};

void Version()
{
  utils::Version();
  std::cout << "Using Qt version " << qVersion() << std::endl;
  std::cout << "qAgate version " << QAGATE_VERSION << std::endl;
}
