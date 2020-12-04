QT       += widgets

TARGET = SH_DeviceListItem
TEMPLATE = lib

DEP = dep_x86
SLNDIR = ../..
CODE = $${SLNDIR}/Code_sdk
INCLUDEDIR = $${CODE}/include/SH_DeviceListItem
SRCDIR = $${CODE}/src/SH_DeviceListItem
UIDIR = $${CODE}/ui/SH_DeviceListItem
LIBDIR = $${SLNDIR}/bin_linux

DESTDIR = $${LIBDIR}/$${DEP}
DEFINES += SH_DEVICELISTITEM_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $${INCLUDEDIR}

LIBS += \

SOURCES += \
        $${SRCDIR}/SH_DeviceListItem.cpp \
        $${SRCDIR}/SecurityHumiture.cpp

HEADERS += \
        $${INCLUDEDIR}/SH_DeviceListItem.h \
        $${INCLUDEDIR}/sh_devicelistitem_global.h \
        $${INCLUDEDIR}/SecurityHumiture.h

FORMS += \
        $${UIDIR}/SecurityHumiture.ui
