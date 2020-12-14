QT       += widgets

TARGET = SH_Controls
TEMPLATE = lib
DEFINES += SH_CONTROLS_LIBRARY

DEP = dep_arm
isEqual(QMAKE_HOST.arch,x86_64){DEP = dep_x86}
message($${DEP})
SLNDIR = ../..
CODE = $${SLNDIR}/Code_sdk
INCLUDEDIR = $${CODE}/include/SH_Controls
SRCDIR = $${CODE}/src/SH_Controls
LIBDIR = $${SLNDIR}/bin_linux/$${DEP}

DESTDIR = $${LIBDIR}
DEFINES += SH_SERIALPORT_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += \
        $${INCLUDEDIR} \

SOURCES += \
        $${SRCDIR}/SHControls.cpp

HEADERS += \
        $${INCLUDEDIR}/sh_controls_global.h \
        $${INCLUDEDIR}/SHControls.h

RESOURCES += \
    images.qrc
