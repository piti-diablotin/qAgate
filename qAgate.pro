#-------------------------------------------------
#
# Project created by QtCreator 2019-01-30T10:17:09
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qAgate
TEMPLATE = app
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNING
DEFINES += HAVE_CONFIG_H

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    qagate.cpp \
    timeline.cpp \
    mediaplayer.cpp \
    view.cpp \
    commandline.cpp \
    logger.cpp \
    qostreamcatcher.cpp \
    settings.cpp \
    abstracttab.cpp \
    mendeleev.cpp \
    atomicdata.cpp

HEADERS += \
    qagate.h \
    timeline.h \
    mediaplayer.h \
    view.h \
    commandline.h \
    logger.h \
    qostreamcatcher.h \
    settings.h \
    abstracttab.h \
    mendeleev.h \
    atomicdata.h

FORMS += \
    qagate.ui \
    timeline.ui \
    mediaplayer.ui \
    view.ui \
    commandline.ui \
    logger.ui \
    settings.ui \
    mendeleev.ui \
    atomicdata.ui

RESOURCES += \
    qagate.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/release/ -lagate
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/debug/ -lagate
else:unix: LIBS += -L$$PWD/../../../../usr/lib/ -lagate

INCLUDEPATH += $$PWD/../../../../usr/include/agate
DEPENDPATH += $$PWD/../../../../usr/include/agate

INCLUDEPATH += /usr/include/freetype2/
