/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : ProgramCounter
FILE   : ProgramCounter.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements a struction splitter
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
--------------------------------------------------------------------------------
Last update on:
26/08/2015 - Eduardo Alves - Initial elaboration
--------------------------------------------------------------------------------
*/
#ifndef PROGRAMCOUNTER_H
#define PROGRAMCOUNTER_H

#include <systemc>

using namespace sc_core;
using namespace sc_dt;

#define INITIAL_PC_ADDRESS 0x00400000   // Initial PC Address

class ProgramCounter : public sc_module {
private:
    unsigned int* lastAddress;
public:
    // Interface
    sc_in<bool>          i_CLK;         // Clock
    sc_in<bool>          i_RST;         // Reset input
    sc_in<sc_uint<32> >  i_DATA_IN;     // Input data channel
    sc_out<sc_uint<32> > o_DATA_OUT;    // Output data channel

    // Module's process
    void p_PC();

    bool simulationEnded;

    SC_HAS_PROCESS(ProgramCounter);
    ProgramCounter(sc_module_name,unsigned int* lastAddress);

};

#endif // PROGRAMCOUNTER_H
