#-------------------------------------------------
#
# Project created by QtCreator 2019-01-30T10:17:09
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = qAgate
VERSION = 1.1.0
TEMPLATE = app
CONFIG += c++11
isEmpty(PREFIX) {
 PREFIX = /usr/
}
isEmpty(PREFIX_AGATE) {
 PREFIX_AGATE = /usr/
}

isEmpty(PREFIX_FREETYPE) {
 PREFIX_FREETYPE = /usr/
}

!isEmpty(PREFIX_SSH) {
  INCLUDEPATH += $$PREFIX_SSH/include/
}

target.path = $$PREFIX/bin
INSTALLS += target

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNING
DEFINES += HAVE_CONFIG_H
DEFINES += QAGATE_VERSION=\\\"$${VERSION}\\\"

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    qagate/main_qagate.cpp \
    qagate/qagate.cpp \
    gui/timeline.cpp \
    gui/mediaplayer.cpp \
    gui/view.cpp \
    gui/commandline.cpp \
    gui/logger.cpp \
    tools/qostreamcatcher.cpp \
    tabs/settings.cpp \
    tabs/abstracttab.cpp \
    dialogs/mendeleev.cpp \
    tools/atomicdata.cpp \
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
    qconducti/qconducti.cpp \
    qdispersion/qdispersion.cpp \
    tabs/tools.cpp \
    dialogs/remotedialog.cpp \
    qtdep/qtdepunitcell.cpp \
    qtdep/qtdepsupercell.cpp \
    qtdep/qtdepoptions.cpp \
    qtdep/qtdep.cpp \
    tools/typatitemdelegate.cpp \
    tools/doubleitemdelegate.cpp \
    tabs/localmode.cpp \
    tabs/densitymode.cpp \
    tabs/phononsmode.cpp \
    dialogs/analyzedialog.cpp \
    qdos/adddos.cpp \
    qdos/qdos.cpp \
    tools/coloritemdelegate.cpp \
    dialogs/about.cpp \
    tabs/multibinittab.cpp

HEADERS += \
    qagate/qagate.h \
    gui/timeline.h \
    gui/mediaplayer.h \
    gui/view.h \
    gui/commandline.h \
    gui/logger.h \
    tools/qostreamcatcher.h \
    tabs/settings.h \
    tabs/abstracttab.h \
    dialogs/mendeleev.h \
    tools/atomicdata.h \
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
    qconducti/qconducti.h \
    qdispersion/qdispersion.h \
    tabs/tools.h \
    dialogs/remotedialog.h \
    qtdep/qtdepunitcell.h \
    qtdep/qtdepsupercell.h \
    qtdep/qtdepoptions.h \
    qtdep/qtdep.h \
    tools/typatitemdelegate.h \
    tools/doubleitemdelegate.h \
    tabs/localmode.h \
    tabs/densitymode.h \
    tabs/phononsmode.h \
    dialogs/analyzedialog.h \
    qdos/adddos.h \
    qdos/qdos.h \
    tools/coloritemdelegate.h \
    dialogs/about.h \
    tabs/multibinittab.h

FORMS += \
    qagate/qagate.ui \
    gui/timeline.ui \
    gui/mediaplayer.ui \
    gui/view.ui \
    gui/commandline.ui \
    gui/logger.ui \
    tabs/settings.ui \
    dialogs/mendeleev.ui \
    tools/atomicdata.ui \
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
    qconducti/qconducti.ui \
    qdispersion/qdispersion.ui \
    tabs/tools.ui \
    dialogs/remotedialog.ui \
    qtdep/qtdepunitcell.ui \
    qtdep/qtdepsupercell.ui \
    qtdep/qtdepoptions.ui \
    qtdep/qtdep.ui \
    tabs/localmode.ui \
    tabs/densitymode.ui \
    tabs/phononsmode.ui \
    dialogs/analyzedialog.ui \
    qdos/adddos.ui \
    qdos/qdos.ui \
    dialogs/about.ui \
    tabs/multibinittab.ui

RESOURCES += \
    qagate/qagate.qrc \
    qdispersion/qdispersion.qrc \
    qconducti/qconducti.qrc \
    qtdep/qtdep.qrc \
    qdos/qdos.qrc

TRANSLATIONS += \
    qtdep/qtdep_fr.ts \
    qtdep/qtdep_it.ts \
    qtdep/qtdep_de.ts \
    qdos/qdos_fr.ts \
    qdos/qdos_it.ts \
    qdos/qdos_de.ts \
    qdispersion/qdispersion_fr.ts \
    qdispersion/qdispersion_it.ts \
    qdispersion/qdispersion_de.ts \
    qconducti/qconducti_fr.ts \
    qconducti/qconducti_it.ts \
    qconducti/qconducti_de.ts \
    qagate/qagate_fr.ts \
    qagate/qagate_it.ts \
    qagate/qagate_de.ts

LIBS += -L$$PREFIX_AGATE/lib/ -lagate


INCLUDEPATH += \
    $$PREFIX_AGATE/include/agate\
    $$PREFIX_FREETYPE/include/freetype2/

DISTFILES += \
    icons/acell.svg \
    icons/alert-circle.svg \
    icons/alert-triangle.svg \
    icons/aliasing.svg \
    icons/angles.svg \
    icons/angle.svg \
    icons/anti-aliasing.svg \
    icons/arrow-down.svg \
    icons/arrow-up.svg \
    icons/average.svg \
    icons/axisABC.svg \
    icons/axis.svg \
    icons/background.svg \
    icons/camangles.svg \
    icons/cell.svg \
    icons/centroid.svg \
    icons/chevrons-down.svg \
    icons/chevrons-right.svg \
    icons/copy.svg \
    icons/delete.svg \
    icons/density.svg \
    icons/download-cloud.svg \
    icons/download.svg \
    icons/energy.svg \
    icons/entropy.svg \
    icons/eye.svg \
    icons/fast-forward.svg \
    icons/file-plus.svg \
    icons/file.svg \
    icons/folder-plus.svg \
    icons/folder-script.svg \
    icons/folder.svg \
    icons/foreground.svg \
    icons/gyration.svg \
    icons/help-circle.svg \
    icons/home.svg \
    icons/image.svg \
    icons/info.svg \
    icons/kinetic.svg \
    icons/local.svg \
    icons/md.svg \
    icons/mendeleev.svg \
    icons/mesh.svg \
    icons/minus-circle.svg \
    icons/minus-square.svg \
    icons/minus.svg \
    icons/move.svg \
    icons/msd.svg \
    icons/noperiodic.svg \
    icons/pause.svg \
    icons/pdf.svg \
    icons/pdos.svg \
    icons/periodic.svg \
    icons/perspective.svg \
    icons/phi.svg \
    icons/phonons.svg \
    icons/play.svg \
    icons/plus-circle.svg \
    icons/plus-square.svg \
    icons/plus.svg \
    icons/positions.svg \
    icons/pressure.svg \
    icons/projection.svg \
    icons/psi.svg \
    icons/qpt.svg \
    icons/refresh-cw.svg \
    icons/repeat.svg \
    icons/rewind.svg \
    icons/rotate-cw.svg \
    icons/rule.svg \
    icons/save-abinit.svg \
    icons/save-poscar.svg \
    icons/save.svg \
    icons/settings.svg \
    icons/shift.svg \
    icons/skip-back.svg \
    icons/skip-forward.svg \
    icons/stress.svg \
    icons/sun.svg \
    icons/supercell.svg \
    icons/thermodynamics.svg \
    icons/thermometer.svg \
    icons/theta.svg \
    icons/time.svg \
    icons/tools.svg \
    icons/trash-2.svg \
    icons/trash.svg \
    icons/typat.svg \
    icons/upload.svg \
    icons/vacf.svg \
    icons/video-off.svg \
    icons/video.svg \
    icons/volume.svg \
    icons/zoom-1.svg \
    icons/zoom-in.svg \
    icons/zoom-out.svg \
    qdispersion/qdispersion.svg \
    qconducti/qconducti.svg \
    qdos/qdos.svg \
    qtdep/qtdep.svg \
    qagate/qagate.svg \
    qagate/qagate.desktop

logo.files = qagate/qagate.svg
logo.path = $$PREFIX/share/qAgate/images/

desktop.files= qagate/qagate.desktop
desktop.path = $$PREFIX/share/applications/

ICON = qagate/qagate.svg

unix:INSTALLS += logo desktop
