/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : Alu
FILE   : Alu.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements a ALU (Arithmetic Logic Unit)
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
--------------------------------------------------------------------------------
Last update on:
31/08/2015 - Eduardo Alves - Initial elaboration
--------------------------------------------------------------------------------
*/

#ifndef ALU_H
#define ALU_H

#include <systemc>

using namespace sc_core;
using namespace sc_dt;

class Alu : public sc_module {
public:
    // Interface
    sc_in<sc_uint<6> >   i_FUNCTION;  // Instruction[5..0]
    sc_in<sc_uint<2> >   i_DATA_OP;   // Alu operation
    sc_in<sc_uint<32> >  i_DATA_A;    // Input A
    sc_in<sc_uint<32> >  i_DATA_B;    // Input B
    sc_out<bool>         o_DATA_ZERO; // Output Zero
    sc_out<sc_uint<32> > o_DATA_OUT;  // Data Output

    // Internal data structures
    sc_signal<sc_uint<4> > w_ALU_OP;

    // Module's process
    void p_updateOp();
    void p_operation();

    SC_HAS_PROCESS(Alu);
    Alu(sc_module_name);
};

#endif // ALU_H
