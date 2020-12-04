QT       += core gui
QT       += sql
QT       += serialport
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEP = dep_x86
SLNDIR = ..
CODE = $${SLNDIR}/Code_sdk
LIBDIR = $${SLNDIR}/bin_linux
INCLUDEDIR = $${CODE}/include

TARGET = SmartHome
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
DESTDIR = $${LIBDIR}
CONFIG += c++11

INCLUDEPATH +=  \
        $${INCLUDEDIR}/SH_Network \
        $${INCLUDEDIR}/SH_Network/protobuf

QMAKE_LFLAGS += \
        -Wl,-rpath,$${LIBDIR}/$${DEP} \
        -Wl,-rpath,$${LIBDIR}

LIBS += \
        -L$${LIBDIR}/ \
        -L$${LIBDIR}/$${DEP}

LIBS += \
        -lSH_Network \
        -lprotobuf

SOURCES += \
        main.cpp \
        SHHomeDlg.cpp \
        SHControls.cpp \
        SHLauncherDlg.cpp \
        SHDeviceMgrDlg.cpp \
        SHAddDeviceDlg.cpp \
        SHDialogMgrDlg.cpp \
        SHSerialPort.cpp \
        SHHouseCtlDlg.cpp \
        HouseCtlCurtain.cpp \
        HouseCtlConditon.cpp \
        HouseCtlLamp.cpp \
        SHPropertyDlg.cpp \
        SHSecurityDlg.cpp \
    SecurityHumiture.cpp \
    SHNeighborDlg.cpp

HEADERS += \
        SHHomeDlg.h \
        SHControls.h \
        SHLauncherDlg.h \
        SHDeviceMgrDlg.h \
        SHAddDeviceDlg.h \
        SHDialogMgrDlg.h \
        SHSerialPort.h \
        SHHouseCtlDlg.h \
        HouseCtlCurtain.h \
        HouseCtlConditon.h \
        HouseCtlLamp.h \
        SHPropertyDlg.h \
        SHSecurityDlg.h \
    SecurityHumiture.h \
    SHNeighborDlg.h

FORMS += \
        SHHomeDlg.ui \
        SHLauncherDlg.ui \
        SHDeviceMgrDlg.ui \
        SHAddDeviceDlg.ui \
        SHDialogMgrDlg.ui \
        SHSerialPort.ui \
        SHHouseCtlDlg.ui \
        HouseCtlCurtain.ui \
        HouseCtlConditon.ui \
        HouseCtlLamp.ui \
        SHPropertyDlg.ui \
        SHSecurityDlg.ui \
    SecurityHumiture.ui \
    SHNeighborDlg.ui

RESOURCES += \
        images.qrc
