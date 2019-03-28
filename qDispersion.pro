#-------------------------------------------------
#
# Project created by QtCreator 2019-03-15T09:27:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = qDispersion
TEMPLATE = app
CONFIG += c++11
isEmpty(PREFIX) {
 PREFIX = /usr/
}
isEmpty(PREFIX_AGATE) {
 PREFIX_AGATE = /usr/
}
isEmpty(PREFIX_SPGLIB) {
 PREFIX_SPGLIB = /usr/
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
    qdispersion/main.cpp \
    qdispersion/qdispersion.cpp \
    tools/energyunitcombo.cpp \
    tools/unitcombo.cpp \
    dialogs/mendeleev.cpp \
    tools/atomicdata.cpp \
    tools/qcustomplot.cpp \
    tools/qplot.cpp

HEADERS += \
    qdispersion/qdispersion.h \
    tools/energyunitcombo.h \
    tools/unitcombo.h \
    dialogs/mendeleev.h \
    tools/atomicdata.h \
    tools/qcustomplot.h \
    tools/qplot.h

FORMS += \
    qdispersion/qdispersion.ui \
    dialogs/mendeleev.ui \
    tools/atomicdata.ui

TRANSLATIONS += \
    qdispersion/qdispersion_fr.ts \
    qdispersion/qdispersion_it.ts \
    qdispersion/qdispersion_de.ts

INCLUDEPATH += $$PREFIX_AGATE/include/agate

RESOURCES += \
    qdispersion/translations.qrc

unix|win32: LIBS += -L$$PREFIX_AGATE/lib/ -L$$PREFIX_SPGLIB/lib -lagate -lsymspg
