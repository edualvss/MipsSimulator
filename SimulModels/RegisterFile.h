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

#include <systemc>
#include <string>

using namespace sc_core;
using namespace sc_dt;

class RegisterFile : public sc_module {
public:

    // Registers names
    static const std::string REG_NAMES[32];

    // Interface
//    sc_in<bool>          i_CLK;         // Clock
    sc_in<bool>         i_WR_ENABLE;   // Input Write enable
    sc_in<sc_uint<5> >  i_RD_ADDRESS_0;// Input Read address 0 (RS field)
    sc_in<sc_uint<5> >  i_RD_ADDRESS_1;// Input Read address 1 (RT field)
    sc_in<sc_uint<5> >  i_WR_ADDRESS;  // Input Write address (RD field)
    sc_in<sc_uint<32> >  i_DATA_IN;     // Input Data
    sc_out<sc_uint<32> > o_DATA_OUT_0;  // Output Data 0
    sc_out<sc_uint<32> > o_DATA_OUT_1;  // Output Data 1

    // Registers
    sc_uint<32> m_REGISTERS[32];       // Register file

    // Module's process
    void p_update();

    // Auxiliar member functions
    int getPosition(std::string );
    sc_uint<32> getRegisterValue(int pos);
    sc_uint<32> getRegisterValue(std::string regName);
    sc_uint<32> *getRegistersValues();
    void debug();

    SC_HAS_PROCESS(RegisterFile);
    RegisterFile(sc_module_name);
};

#endif // REGISTERFILE_H
