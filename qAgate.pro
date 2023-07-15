#-------------------------------------------------
#
# Project created by QtCreator 2019-01-30T10:17:09
#
#-------------------------------------------------

QT       += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
greaterThan(QT_MAJOR_VERSION, 5): QT += openglwidgets

TARGET = qAgate
VERSION = 1.2.3
TEMPLATE = app
CONFIG += c++11
isEmpty(PREFIX) {
 PREFIX = /usr/
 macx:PREFIX = /usr/local
}
isEmpty(PREFIX_AGATE) {
 PREFIX_AGATE = /usr/
 macx:PREFIX_AGATE = /usr/local
}

isEmpty(PREFIX_FREETYPE) {
 PREFIX_FREETYPE = /usr/
 macx:PREFIX_FREETYPE = /usr/local
}

!isEmpty(PREFIX_SSH) {
  INCLUDEPATH += $$PREFIX_SSH/include/
}

macx:INCLUDEPATH += /usr/local/include

target.path = $$PREFIX/bin
INSTALLS += target

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNING
DEFINES += HAVE_CONFIG_H
DEFINES += QAGATE_VERSION=\\\"$${VERSION}\\\"
DEFINES += INPUT_FILES_FILTER="\"\\\"Abinit (*.abi *.abo *_OUT.nc *_HIST *_HIST.nc *_DDB *_DEN *_OPT);;VASP (POSCAR OUTCAR);;CIF (*.cif);;XML (*.xml);;XYZ (*.xyz);; YAML (*.yaml);; All (*)\\\"\""
DEFINES += DDB_FILES_FILTER="\"\\\"Abinit (*.abo *_DDB *_PHBST.nc);;VASP (OUTCAR);; YAML (*.yaml);; All (*)\\\"\""
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    dialogs/mbgeneratordialog.cpp \
    dialogs/stressdialog.cpp \
    dialogs/straindialog.cpp \
    qagate/main_qagate.cpp \
    qagate/qagate.cpp \
    gui/timeline.cpp \
    gui/mediaplayer.cpp \
    gui/view.cpp \
    gui/commandline.cpp \
    gui/logger.cpp \
    dialogs/polarizationdialog.cpp \
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
    tabs/multibinittab.cpp \
    tools/versionchecker.cpp \

HEADERS += \
    dialogs/mbgeneratordialog.h \
    dialogs/stressdialog.h \
    dialogs/straindialog.h \
    qagate/qagate.h \
    gui/timeline.h \
    gui/mediaplayer.h \
    gui/view.h \
    gui/commandline.h \
    gui/logger.h \
    dialogs/polarizationdialog.h \
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
    tabs/multibinittab.h \
    tools/versionchecker.h \

FORMS += \
    dialogs/mbgeneratordialog.ui \
    dialogs/stressdialog.ui \
    dialogs/straindialog.ui \
    qagate/qagate.ui \
    gui/timeline.ui \
    gui/mediaplayer.ui \
    gui/view.ui \
    gui/commandline.ui \
    gui/logger.ui \
    dialogs/polarizationdialog.ui \
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
    icons/eye-off.svg \
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
    icons/polarization.svg \
    icons/positions.svg \
    icons/pressure.svg \
    icons/projection.svg \
    icons/psi.svg \
    icons/qpt.svg \
    icons/refresh-cw.svg \
    icons/repeat.svg \
    icons/rewind.svg \
    icons/rotate-cw.svg \
    icons/rotations.svg \
    icons/rule.svg \
    icons/save-abinit.svg \
    icons/save-poscar.svg \
    icons/save.svg \
    icons/settings.svg \
    icons/shift.svg \
    icons/skip-back.svg \
    icons/skip-forward.svg \
    icons/stress.svg \
    icons/strain.svg \
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
    icons/xy.svg \
    icons/zoom-1.svg \
    icons/zoom-in.svg \
    icons/zoom-out.svg \
    qdispersion/qdispersion.svg \
    qconducti/qconducti.svg \
    qdos/qdos.svg \
    qtdep/qtdep.svg \
    qagate/qagate.svg \
    qagate/qagate.icns \
    qagate/qagate.desktop \
    qagate/qagate_fr.qm \
    qagate/qagate_it.qm \
    qagate/qagate_de.qm \
    qconducti/qconducti_de.qm \
    qconducti/qconducti_fr.qm \
    qconducti/qconducti_it.qm \
    qdispersion/qdispersion_de.qm \
    qdispersion/qdispersion_fr.qm \
    qdispersion/qdispersion_it.qm \
    qdos/qdos_de.qm \
    qdos/qdos_fr.qm \
    qdos/qdos_it.qm \
    qtdep/qtdep_de.qm \
    qtdep/qtdep_fr.qm \
    qtdep/qtdep_it.qm


logo.files = qagate/qagate.svg
logo.path = $$PREFIX/share/qAgate/images/

desktop.files= qagate/qagate.desktop
desktop.path = $$PREFIX/share/applications/

unix:ICON = qagate/qagate.svg
macx:ICON = qagate/qagate.icns

unix:INSTALLS += logo desktop
