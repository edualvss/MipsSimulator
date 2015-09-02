/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : Instruction
FILE   : Instruction.cpp
--------------------------------------------------------------------------------
*/

#include "Instruction.h"

#include <stdio.h>

#include <iostream>

const std::string Instruction::INSTRUCTION_FORMAT_NAME[3] = {
    "R","I","J"
};

const std::string Instruction::ADDRESSING_MODE_NAME[5] = {
    "Register","Base","Immediate","PC-relative","Pseudodirect"
};

Instruction::Instruction(unsigned int instruction) : instruction(instruction) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor Instruction" << std::endl;
#endif

}

#include <iostream>
#include <sstream>
#include <string>
#include <ostream>
void Instruction::debug() {
#ifdef DEBUG_METHODS
    std::cout << "Instruction::debug" << std::endl;
#endif

    std::string str;
    std::stringstream ss;

    ss << "0x" << std::uppercase << std::hex;

    switch( this->format ) {
        case R:
            str = insMnemonic + "\t" + rdRegName + ", " + rsRegName + ", " + rtRegName;
            if( (insMnemonic.compare("sll") == 0) || (insMnemonic.compare("srl") == 0) ) {
                ss << _shamt;
                str = insMnemonic + "\t" + rdRegName + ", " + rsRegName + ", " + ss.str();
            } else {
                if( insMnemonic.compare("jr") == 0 ) {
                    str = insMnemonic + "\t" + rsRegName;
                } else if(insMnemonic.compare("syscall") == 0) {
                    str = "syscall";
                }
            }
            break;
        case I:
            if( (insMnemonic.compare("lw") == 0) || (insMnemonic.compare("sw") == 0) ) {
                ss << _imed16;
                str = insMnemonic + "\t" + rtRegName + ", " + ss.str() + "(" + rsRegName + ")";
            } else {
                ss << _imed16;
                str = insMnemonic + "\t" + rtRegName + ", " + rsRegName + "," + ss.str();
            }
            break;
        case J:
            ss << _imed26;
            str = insMnemonic + "\t" + ss.str();

            break;
    }

    std::cout << "\nInstruction is: " << str << std::endl;

}
