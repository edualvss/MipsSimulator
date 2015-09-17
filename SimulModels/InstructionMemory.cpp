/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : InstructionMemory
FILE   : InstructionMemory.cpp
--------------------------------------------------------------------------------
*/

#include "InstructionMemory.h"
#include "ProgramCounter.h"

InstructionMemory::InstructionMemory(sc_module_name mn) : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor InstructionMemory" << std::endl;
#endif

    SC_METHOD(p_update);
    dont_initialize();
    sensitive << i_DATA_ADDRESS;

}

////////////////////////////////////////////////////////////////////////////////
void InstructionMemory::p_update() {
////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_METHODS
    std::cout << "InstructionMemory::p_update" << std::endl;
#endif

    if( m_MEMORY.empty() ) {
        printf("WARNING: Instruction Memory is EMPTY");
        return;
    }

    sc_uint<32> v_ADDRESS;

    v_ADDRESS = i_DATA_ADDRESS.read();

    v_ADDRESS -= INITIAL_PC_ADDRESS;

    v_ADDRESS = v_ADDRESS / 4;

    o_DATA_INSTRUCTION.write( m_MEMORY[v_ADDRESS] );

}

bool InstructionMemory::initialize(const char *filename) {
#ifdef DEBUG_METHODS
    std::cout << "InstructionMemory::initialize" << std::endl;
#endif

    FILE* file;

    if( (file = fopen(filename,"rt") ) == NULL ) {
        printf("Falha ao abrir arquivo para carregar a memoria de instrucoes");
        return false;
    }

    this->m_MEMORY.clear();

    int scanResult = 0;
    do {
        unsigned int hexValue;
        scanResult = fscanf(file,"%x",&hexValue);
        m_MEMORY.push_back(hexValue);
    } while( scanResult != EOF );

    fclose(file);

    m_MEMORY.pop_back();

    lastAddress = (m_MEMORY.size() - 1) * 4 + INITIAL_PC_ADDRESS;

    return true;
}

std::vector<sc_uint<32> > InstructionMemory::getInstructions() {
#ifdef DEBUG_METHODS
    std::cout << "InstructionMemory::getInstructions" << std::endl;
#endif
    return m_MEMORY;
}

int InstructionMemory::getInstructionCount() {
#ifdef DEBUG_METHODS
    std::cout << "InstructionMemory::getInstructionCount" << std::endl;
#endif

    return m_MEMORY.size();
}

void InstructionMemory::debug() {
#ifdef DEBUG_METHODS
    std::cout << "InstructionMemory::debug" << std::endl;
#endif

    if( m_MEMORY.empty() ) {
        printf("\nInstruction Memory Empty");
        return;
    }
    printf("\nInstruction Memory Contents");
    for(unsigned int i = 0; i < m_MEMORY.size(); i++) {
        unsigned int value = m_MEMORY.at(i);
        printf("\nAddress[%#.8x]: %#.8x, index: %u",((i * 4) + INITIAL_PC_ADDRESS),value,i);
    }

}
