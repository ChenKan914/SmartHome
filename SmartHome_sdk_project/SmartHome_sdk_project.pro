DEP = dep_arm
DEFINES += UBUNTU
contains(DEFINES, UBUNTU){
    message(ubuntu)
    DEP = dep_x86
}

TEMPLATE = subdirs
SUBDIRS += \
        SH_Network \
        SH_DeviceListItem





