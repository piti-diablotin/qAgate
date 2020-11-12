#-------------------------------------------------
#
# Project created by QtCreator 2019-09-20T10:30:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = qDOS
TEMPLATE = app
CONFIG += c++11
isEmpty(PREFIX) {
 PREFIX = /usr/
}
isEmpty(PREFIX_AGATE) {
 PREFIX_AGATE = /usr/
}
target.path = $$PREFIX/bin
INSTALLS += target

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += HAVE_CONFIG_H

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        qdos/main_qdos.cpp \
        qdos/qdos.cpp \
    tools/energyunitcombo.cpp \
    tools/qcustomplot.cpp \
    tools/qplot.cpp \
    tools/unitcombo.cpp \
    qdos/adddos.cpp \
    tools/coloritemdelegate.cpp

HEADERS += \
        qdos/qdos.h \
    tools/energyunitcombo.h \
    tools/qcustomplot.h \
    tools/qplot.h \
    tools/unitcombo.h \
    qdos/adddos.h \
    tools/coloritemdelegate.h

FORMS += \
        qdos/qdos.ui \
    qdos/adddos.ui

INCLUDEPATH += $$PREFIX_AGATE/include/agate

RESOURCES += \
    qdos/qdos.qrc

TRANSLATIONS += \
  qdos/qdos_fr.ts \
  qdos/qdos_it.ts \
  qdos/qdos_de.ts

unix|win32: LIBS += -L$$PREFIX_AGATE/lib/ -lagate

DISTFILES += \
    qdos/qdos.svg \
    qdos/qdos.desktop

logo.files = qdos/qdos.svg
logo.path = $$PREFIX/share/qAgate/images/

desktop.files= qdos/qdos.desktop
desktop.path = $$PREFIX/share/applications/

ICON = qdos/qdos.svg

unix:INSTALLS += logo desktop
