QT       += widgets network

TARGET = SH_Network
TEMPLATE = lib

DEP = dep_x86
SLNDIR = ../..
CODE = $${SLNDIR}/Code_sdk
INCLUDEDIR = $${CODE}/include/SH_Network
SRCDIR = $${CODE}/src/SH_Network
LIBDIR = $${SLNDIR}/bin_linux/$${DEP}

DESTDIR = $${LIBDIR}
DEFINES += SH_NETWORK_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += \
        $${INCLUDEDIR} \
        $${INCLUDEDIR}/protobuf

LIBS += -L$${LIBDIR}
LIBS += -lprotobuf

SOURCES += \
        $${SRCDIR}/SHNetworkMessage.cpp \
        $${SRCDIR}/SHTcpSocket.cpp \
        $${SRCDIR}/message.pb.cc


HEADERS += \
        $${INCLUDEDIR}/SHNetworkMessage.h \
        $${INCLUDEDIR}/SHTcpSocket.h \
        $${INCLUDEDIR}/sh_network_global.h \
        $${INCLUDEDIR}/message.pb.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}
