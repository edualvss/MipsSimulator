/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : InstructionMemory
FILE   : InstructionMemory.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements a instruction memory
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
--------------------------------------------------------------------------------
Last update on:
26/08/2015 - Eduardo Alves - Initial elaboration
--------------------------------------------------------------------------------
*/
#ifndef INSTRUCTIONMEMORY_H
#define INSTRUCTIONMEMORY_H

#include <systemc>

using namespace sc_core;
using namespace sc_dt;

class InstructionMemory : public sc_module {
private:
    // Internal memory
    std::vector<sc_uint<32> > m_MEMORY;      // Memory

public:

    // Interface
    sc_in<sc_uint<32> >  i_DATA_ADDRESS;     // Input address
    sc_out<sc_uint<32> > o_DATA_INSTRUCTION; // Output data instruction

    // Auxiliar member function
    bool initialize(const char* filename);
    std::vector<sc_uint<32> > getInstructions();
    int getInstructionCount();
    void debug();

    // Module's process
    void p_update();

    SC_HAS_PROCESS(InstructionMemory);
    InstructionMemory(sc_module_name);
};

#endif // INSTRUCTIONMEMORY_H
