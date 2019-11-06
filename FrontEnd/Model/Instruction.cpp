/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : Instruction
FILE   : Instruction.cpp
--------------------------------------------------------------------------------
*/

#include "Instruction.h"

#include <iostream>
#include <cstdio>
#include <cstring>

const char* Instruction::INSTRUCTION_FORMAT_NAME[3] = {
    "R","I","J"
};

const char* Instruction::ADDRESSING_MODE_NAME[5] = {
    "Register","Base","Immediate","PC-relative","Pseudodirect"
};

Instruction::Instruction(unsigned int instruction) : instruction(instruction) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor Instruction" << std::endl;
#endif

}

char *Instruction::getFormattedInstruction() {
#ifdef DEBUG_METHODS
    std::cout << "Instruction::getFormattedInstruction" << std::endl;
#endif

    char* str = new char[30];

    if( strcmp(insMnemonic,"nop") == 0) {
        sprintf(str,"%s",insMnemonic);
        return str;
    }

    switch( this->format ) {
        case R:
            sprintf(str,"%s\t%s, %s, %s",insMnemonic,rdRegName,rsRegName,rtRegName);
            if( (strcmp(insMnemonic,"sll") == 0) || (strcmp(insMnemonic,"srl") == 0) ) {
                sprintf(str,"%s\t%s, %s, 0x%X",insMnemonic,rdRegName,rsRegName,_shamt);
            } else {
                if( strcmp(insMnemonic,"jr") == 0 ) {
                    sprintf(str,"%s\t%s",insMnemonic,rsRegName);
                } else if(strcmp(insMnemonic,"syscall") == 0) {
                    sprintf(str,"syscall");
                }
            }
            break;
        case I:
            if( (strcmp(insMnemonic,"lw") == 0) || (strcmp(insMnemonic,"sw") == 0) ) {
                sprintf(str,"%s\t%s, 0x%X(%s)",insMnemonic,rtRegName,_imed16,rsRegName);
            } else {
                sprintf(str,"%s\t%s, %s, 0x%X",insMnemonic,rtRegName,rsRegName,_imed16);
            }
            break;
        case J:
            sprintf(str,"%s\t0x%X",insMnemonic,_imed26);
            break;
    }

    return str;
}

void Instruction::debug() {
#ifdef DEBUG_METHODS
    std::cout << "Instruction::debug" << std::endl;
#endif

    char* formattedIns = getFormattedInstruction();
    std::cout << "\nInstruction is: " << formattedIns << std::endl;

    delete[] formattedIns;
}
