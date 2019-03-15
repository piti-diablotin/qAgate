#-------------------------------------------------
#
# Project created by QtCreator 2019-01-22T15:17:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = qConducti
TEMPLATE = app
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        conducti/main.cpp \
        conducti/qconducti.cpp \
        tools/qcustomplot.cpp \
        tools/qplot.cpp \
        qostreamcatcher.cpp

HEADERS += \
        conducti/qconducti.h \
        tools/qcustomplot.h \
        tools/qplot.h \
        qostreamcatcher.h

FORMS += \
        conducti/qconducti.ui

TRANSLATIONS += qconducti_fr.ts qconducti_it.ts qconducti_de.ts

INCLUDEPATH += /usr/include/agate

RESOURCES += \
    conducti/translations.qrc

unix|win32: LIBS += -lagate -lsymspg
