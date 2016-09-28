/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : RegisterFile
FILE   : RegisterFile.cpp
--------------------------------------------------------------------------------
*/

#include "RegisterFile.h"

#include <cstring>

#ifdef DEBUG_METHODS
    #include <iostream>
#endif

// Register names by position in array
const char* RegisterFile::REG_NAMES[32] =
        {"$zero","$at","$v0","$v1","$a0",
        "$a1","$a2","$a3","$t0","$t1",
        "$t2","$t3","$t4","$t5","$t6",
        "$t7","$s0","$s1","$s2","$s3",
        "$s4","$s5","$s6","$s7","$t8",
        "$t9","$k0","$k1","$gp","$sp",
        "$fp","$ra"};

RegisterFile::RegisterFile() {
#ifdef DEBUG_METHODS
    std::cout << "Constructor RegisterFile" << std::endl;
#endif

}

int RegisterFile::getPosition(const char *regName) {
#ifdef DEBUG_METHODS
    std::cout << "RegisterFile::getPosition" << std::endl;
#endif

    for( int i = 0; i < 32; i++ ) {
        if( strcmp(regName,REG_NAMES[i]) == 0 ) {
            return i;
        }
    }

    return -1;
}
