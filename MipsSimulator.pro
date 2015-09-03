TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    SimulModels/ProgramCounter.h \
    SimulModels/RegisterFile.h \
    SimulModels/InstructionMemory.h \
    SimulModels/Control.h \
    SimulModels/DataMemory.h \
    SimulModels/Decoder.h \
    SimulModels/Instruction.h \
    SimulModels/Mux2x1.h \
    SimulModels/Alu.h \
    SimulModels/PC_Control.h \
    SimulModels/MipsTop.h \
    SimulModels/Testbench.h

SOURCES += main.cpp \
    SimulModels/ProgramCounter.cpp \
    SimulModels/RegisterFile.cpp \
    SimulModels/InstructionMemory.cpp \
    SimulModels/Control.cpp \
    SimulModels/DataMemory.cpp \
    SimulModels/Decoder.cpp \
    SimulModels/Instruction.cpp \
    SimulModels/Mux2x1.cpp \
    SimulModels/Alu.cpp \
    SimulModels/PC_Control.cpp \
    SimulModels/MipsTop.cpp

CONFIG -= debug_and_release

TARGET  = SimMips
DESTDIR = bin
OBJECTS_DIR = objs

SYSTEMC_PATH= c:/SystemC

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
