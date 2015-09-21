/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : Decoder
FILE   : Decoder.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements a instruction decoder
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
--------------------------------------------------------------------------------
Last update on:
27/08/2015 - Eduardo Alves - Initial elaboration
--------------------------------------------------------------------------------
*/
#ifndef DECODER_H
#define DECODER_H

#include <systemc>

using namespace sc_core;
using namespace sc_dt;

class Instruction;

class Decoder : public sc_module {
public:

    // Interface
    sc_in<sc_uint<32> >  i_DATA_IN;         // Input data channel (output instruction memory)
    sc_out<sc_uint<26> > o_DATA_IMED26;     // Output data imediate26 (input shift left)
    sc_out<sc_uint<6> >  o_DATA_OP;         // Output data operation (input control)
    sc_out<sc_uint<5> >  o_DATA_RS;         // Output data rs (input addr_rd_0 of register file)
    sc_out<sc_uint<5> >  o_DATA_RT;         // Output data rt (input addr_rd_1 of register file)
    sc_out<sc_uint<5> >  o_DATA_RD;         // Output data rd (input 1 mux2 before addr_wr of register file)
    sc_out<sc_uint<16> > o_DATA_IMED16;     // Output data imediate16 (input signal extender)
    sc_out<sc_uint<5> >  o_DATA_SHAMT;      // Output data shamt (shif amount)
    sc_out<sc_uint<6> >  o_DATA_FUNCTION;   // Output data function (input alu control)

    // There is no internal data structures

    // Module's process
    void p_decode();

    // Auxiliar member functions
    Instruction* getInstructionDecoded();
    static Instruction* getInstructionDecoded(unsigned int instruction);
    static Instruction* getInstructionDecoded(Instruction* ins);

    SC_HAS_PROCESS(Decoder);
    Decoder(sc_module_name);

};

#endif // DECODER_H
