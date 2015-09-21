TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = SysMips
DESTDIR = bin

SYSTEMC_PATH= c:/SystemC

OBJECTS_DIR = objs

SOURCES += main.cpp \
    SimulModels/Alu.cpp \
    SimulModels/Control.cpp \
    SimulModels/DataMemory.cpp \
    SimulModels/Decoder.cpp \
    SimulModels/InstructionMemory.cpp \
    SimulModels/MipsTop.cpp \
    SimulModels/Mux2x1.cpp \
    SimulModels/PC_Control.cpp \
    SimulModels/ProgramCounter.cpp \
    SimulModels/RegisterFile.cpp \
    SimulModels/SystemMonitor.cpp \
    Model/CycleStatus.cpp \
    Model/Instruction.cpp

INCLUDEPATH = $${SYSTEMC_PATH}/include

win32 {
  LIBS += $${SYSTEMC_PATH}/lib-mingw/libsystemc.a
}

unix {
    contains(QMAKE_HOST.arch, x86_64) {
         # 64-bit Unix
        mac {
            LIBS = -L$${SYSTEMC_PATH}/lib-macosx64 -lsystemc -Xlinker -rpath -Xlinker $${SYSTEMC_PATH}/lib-macosx64
        } else {
            LIBS = -L$${SYSTEMC_PATH}/lib-linux64 -lsystemc -Xlinker -rpath -Xlinker $${SYSTEMC_PATH}/lib-linux64
        }
    } else {
        # 32-bit Unix not OS X
        LIBS = -L$${SYSTEMC_PATH}/lib-linux -lsystemc -Xlinker -rpath -Xlinker $${SYSTEMC_PATH}/lib-linux
    }
}

HEADERS += \
    SimulModels/Alu.h \
    SimulModels/Control.h \
    SimulModels/DataMemory.h \
    SimulModels/Decoder.h \
    SimulModels/InstructionMemory.h \
    SimulModels/MipsTop.h \
    SimulModels/Mux2x1.h \
    SimulModels/PC_Control.h \
    SimulModels/ProgramCounter.h \
    SimulModels/RegisterFile.h \
    SimulModels/SystemMonitor.h \
    SimulModels/testbench.h \
    Model/CycleStatus.h \
    Model/Instruction.h

# OS X specific settings
mac {
    ICON = chip_icon.icns
    FRAMEWORK_HEADERS.version = "1.0"
    QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
}

# Win32 specific settings
win32 {
    QMAKE_TARGET_COMPANY = "EAS"
    QMAKE_TARGET_PRODUCT = SysMips
    QMAKE_TARGET_DESCRIPTION = "A SystemC Mips simulator"
    QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2015 Eduardo Alves da Silva"
    RC_ICONS = chip_icon.ico
}

CONFIG -= debug_and_release

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
