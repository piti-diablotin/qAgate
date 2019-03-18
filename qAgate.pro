#-------------------------------------------------
#
# Project created by QtCreator 2019-01-30T10:17:09
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = qAgate
VERSION = 1.0.0
TEMPLATE = app
CONFIG += c++11
isEmpty(PREFIX) {
 PREFIX = /usr/
}
isEmpty(PREFIX_AGATE) {
 PREFIX = /usr/
}
isEmpty(PREFIX_FREETYPE) {
 PREFIX = /usr/
}
target.path = $$PREFIX/bin
INSTALLS += target

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
    tabs/settings.cpp \
    tabs/abstracttab.cpp \
    mendeleev.cpp \
    atomicdata.cpp \
    tabs/visuals.cpp \
    tabs/home.cpp \
    dialogs/dumpdialog.cpp \
    dialogs/writedialog.cpp \
    tools/distanceunitcombo.cpp \
    tools/unitcombo.cpp \
    dialogs/spgdialog.cpp \
    dialogs/supercelldialog.cpp \
    dialogs/shiftdialog.cpp \
    dialogs/movedialog.cpp \
    dialogs/typatdialog.cpp \
    tools/qplot.cpp \
    tools/qcustomplot.cpp \
    dialogs/plotwindow.cpp \
    tabs/md.cpp \
    dialogs/atomdialog.cpp \
    dialogs/plandialog.cpp \
    dialogs/pdfdialog.cpp \
    tools/energyunitcombo.cpp \
    dialogs/smearingdialog.cpp \
    qConducti/qconducti.cpp \
    tabs/tools.cpp

HEADERS += \
    qagate.h \
    timeline.h \
    mediaplayer.h \
    view.h \
    commandline.h \
    logger.h \
    qostreamcatcher.h \
    tabs/settings.h \
    tabs/abstracttab.h \
    mendeleev.h \
    atomicdata.h \
    tabs/visuals.h \
    tabs/home.h \
    dialogs/dumpdialog.h \
    dialogs/writedialog.h \
    tools/distanceunitcombo.h \
    tools/unitcombo.h \
    dialogs/spgdialog.h \
    dialogs/supercelldialog.h \
    dialogs/shiftdialog.h \
    dialogs/movedialog.h \
    dialogs/typatdialog.h \
    tools/qplot.h \
    tools/qcustomplot.h \
    dialogs/plotwindow.h \
    tabs/md.h \
    dialogs/atomdialog.h \
    dialogs/plandialog.h \
    dialogs/pdfdialog.h \
    tools/energyunitcombo.h \
    dialogs/smearingdialog.h \
    qConducti/qconducti.h \
    tabs/tools.h

FORMS += \
    qagate.ui \
    timeline.ui \
    mediaplayer.ui \
    view.ui \
    commandline.ui \
    logger.ui \
    tabs/settings.ui \
    mendeleev.ui \
    atomicdata.ui \
    tabs/visuals.ui \
    tabs/home.ui \
    dialogs/dumpdialog.ui \
    dialogs/writedialog.ui \
    dialogs/spgdialog.ui \
    dialogs/supercelldialog.ui \
    dialogs/shiftdialog.ui \
    dialogs/movedialog.ui \
    dialogs/typatdialog.ui \
    dialogs/plotwindow.ui \
    tabs/md.ui \
    dialogs/atomdialog.ui \
    dialogs/plandialog.ui \
    dialogs/pdfdialog.ui \
    dialogs/smearingdialog.ui \
    qConducti/qconducti.ui \
    tabs/tools.ui

RESOURCES += \
    qagate.qrc

TRANSLATIONS += \
  qagate_fr.ts \
  qagate_it.ts \
  qagate_de.ts

LIBS += -L$$PREFIX_AGATE/ -lagate -lsymspg

INCLUDEPATH += \
    $$PREFIX_AGATE/include/agate\
    $$PREFIX_FREETYPE/include/freetype2/
