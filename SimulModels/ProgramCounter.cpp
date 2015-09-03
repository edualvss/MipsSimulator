/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : ProgramCounter
FILE   : ProgramCounter.cpp
--------------------------------------------------------------------------------
*/

#include "ProgramCounter.h"

ProgramCounter::ProgramCounter(sc_module_name mn, unsigned int lastAddress) : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor ProgramCounter" << std::endl;
#endif

    this->lastAddress = lastAddress;
    this->simulationEnded = false;

    o_DATA_OUT.initialize(INITIAL_PC_ADDRESS);
    SC_METHOD(p_PC);
    dont_initialize();
    sensitive << i_CLK.pos() << i_RST;

}

////////////////////////////////////////////////////////////////////////////////
void ProgramCounter::p_PC() {
////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_METHODS
    std::cout << "ProgramCounter::p_PC" << std::endl;
#endif

    sc_uint<32> v_DATA_IN;
    v_DATA_IN = i_DATA_IN.read();

    if( v_DATA_IN > lastAddress ) { // After last instruction
        sc_stop();                  // End simulation
        simulationEnded = true;
    }

    if( i_RST ) {
        o_DATA_OUT.write(INITIAL_PC_ADDRESS);
    } else {
        o_DATA_OUT.write(i_DATA_IN.read());
    }

}
