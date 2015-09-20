/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : RegisterFile
FILE   : RegisterFile.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements a register file
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
--------------------------------------------------------------------------------
Last update on:
26/08/2015 - Eduardo Alves - Initial elaboration
--------------------------------------------------------------------------------
*/
#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <string>

class RegisterFile {
    RegisterFile();
public:

    // Registers names
    static const std::string REG_NAMES[32];

    // Auxiliar functions
    static int getPosition(std::string );

};

#endif // REGISTERFILE_H
