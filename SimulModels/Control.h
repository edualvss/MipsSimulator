/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : Control
FILE   : Control.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements a control unit
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
--------------------------------------------------------------------------------
Last update on:
26/08/2015 - Eduardo Alves - Initial elaboration
--------------------------------------------------------------------------------
*/
#ifndef CONTROL_H
#define CONTROL_H

#include <systemc>

using namespace sc_core;
using namespace sc_dt;

class Control : public sc_module {
public:

    // Interface
    sc_in<sc_uint<6> >  i_DATA_OP;          // Input data channel[5:0]
    sc_out<sc_uint<2> > o_DATA_ALU_OP;      // Output data channel ALUOp[1:0]
    sc_out<bool>        o_DATA_REG_DST;     // Output data channel RegDst
    sc_out<bool>        o_DATA_ALU_SRC;     // Output data channel ALUSrc
    sc_out<bool>        o_DATA_MEM_TO_REG;  // Output data channel MemtoReg
    sc_out<bool>        o_DATA_REG_WRITE;   // Output data channel RegWrite
    sc_out<bool>        o_DATA_MEM_READ;    // Output data channel MemRead
    sc_out<bool>        o_DATA_MEM_WRITE;   // Output data channel MemWrite
    sc_out<bool>        o_DATA_BRANCH;      // Output data channel Branch
    sc_out<bool>        o_DATA_DVI;         // Output data channel DVI

    // There is no internal data structures

    // Module's process
    void p_control();

    SC_HAS_PROCESS(Control);
    Control(sc_module_name);
};

#endif // CONTROL_H
