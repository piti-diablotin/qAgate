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
        qconducti/main_qconducti.cpp \
        qconducti/qconducti.cpp \
        tools/qcustomplot.cpp \
        tools/qplot.cpp \
        tools/qostreamcatcher.cpp

HEADERS += \
        qconducti/qconducti.h \
        tools/qcustomplot.h \
        tools/qplot.h \
        tools/qostreamcatcher.h

FORMS += \
        qconducti/qconducti.ui

TRANSLATIONS += \
        qconducti/qconducti_fr.ts \
        qconducti/qconducti_it.ts \
        qconducti/qconducti_de.ts

INCLUDEPATH += $$PREFIX_AGATE/include/agate

RESOURCES += \
    qconducti/qconducti.qrc

unix|win32: LIBS += -L$$PREFIX_AGATE/lib/ -lagate 

DISTFILES += \
    qconducti/qconducti.svg \
    qconducti/qconducti.desktop

logo.files = qconducti/qconducti.svg
logo.path = /usr/share/qAgate/images/

desktop.files= qconducti/qconducti.desktop
desktop.path = /usr/share/applications/

ICON = qagate/qconducti.svg

unix:INSTALLS += logo desktop
