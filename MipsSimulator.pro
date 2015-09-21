QT = core gui widgets
TEMPLATE = app

HEADERS += \
    View/MainWindow.h \
    Control/AppControl.h \
    Model/Instruction.h \
    Model/CycleStatus.h \
    Model/Decoder.h \
    Model/RegisterFile.h \
    View/AboutWindow.h

SOURCES += main.cpp \
    View/MainWindow.cpp \
    Control/AppControl.cpp \
    Model/Instruction.cpp \
    Model/CycleStatus.cpp \
    Model/Decoder.cpp \
    Model/RegisterFile.cpp \
    View/AboutWindow.cpp

CONFIG -= debug_and_release

TARGET  = SimMips
DESTDIR = bin
OBJECTS_DIR = objs

# OS X specific settings
mac {
    ICON = app_icon.icns
    FRAMEWORK_HEADERS.version = "1.0"
    QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
}

# Win32 specific settings
win32 {
    QMAKE_TARGET_COMPANY = "EAS"
    QMAKE_TARGET_PRODUCT = MipsSimulator
    QMAKE_TARGET_DESCRIPTION = "A front-end to SystemC Mips simulator"
    QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2015 Eduardo Alves da Silva"
    RC_ICONS = app_icon.ico
}

# Debug define
CONFIG(debug, debug|release) {
    # Application target for debug
    CONFIG -= debug release
    CONFIG += debug
}
# Release define
CONFIG(release, debug|release) {
    # Application target for release
    CONFIG -= debug release
    CONFIG += release
}

debug {
    CONFIG  += warn_on
    DEFINES += DEBUG_METHODS
}

FORMS += \
    View/MainWindow.ui \
    View/AboutWindow.ui

RESOURCES += \
    View/media.qrc
