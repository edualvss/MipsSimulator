/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : Alu
FILE   : Alu.cpp
--------------------------------------------------------------------------------
*/

#include "Alu.h"

Alu::Alu(sc_module_name mn) : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor: ALU" << std::endl;
#endif

    SC_METHOD(p_updateOp);
    dont_initialize();
    sensitive << i_DATA_OP << i_FUNCTION;

    SC_METHOD(p_operation);
    dont_initialize();
    sensitive << w_ALU_OP << i_DATA_A << i_DATA_B;

}

void Alu::p_updateOp() {
#ifdef DEBUG_METHODS
    std::cout << "Alu::p_updateOp" << std::endl;
#endif

    sc_uint<6> v_FUNCTION;
    sc_uint<2> v_OP;
    v_FUNCTION = i_FUNCTION.read();
    v_OP = i_DATA_OP.read();

    sc_uint<4> v_OUT;

    v_OUT[3] = 0;
    v_OUT[2] = v_OP[0] or (v_OP[1] and v_FUNCTION[1]);
    v_OUT[1] = !v_OP[1] or !v_FUNCTION[2];
    v_OUT[0] = v_OP[1] and (v_FUNCTION[3] or v_FUNCTION[0]);

    w_ALU_OP.write(v_OUT);
}

void Alu::p_operation() {
#ifdef DEBUG_METHODS
    std::cout << "Alu::p_operation" << std::endl;
#endif

    sc_uint<4>  v_OP;
    sc_uint<32> v_A;
    sc_uint<32> v_B;
    sc_uint<33> v_RESULT;

    v_OP = w_ALU_OP.read();
    v_A = i_DATA_A.read();
    v_B = i_DATA_B.read();


    switch(v_OP) {
        case 0: // 0000
            v_RESULT = v_A & v_B;
            break;
        case 1: // 0001
            v_RESULT = v_A | v_B;
            break;
        case 2: // 0010
            v_RESULT = v_A + v_B;
            break;
        case 6: // 0110
            v_RESULT = v_A - v_B;
            break;
        case 7: // 0111
            v_RESULT = v_A < v_B ? 1 : 0;
            break;
        case 12:// 1100
            v_RESULT = ~(v_A | v_B); // result is nor
            break;
        default:
            v_RESULT = 0;
    }

    o_DATA_ZERO.write( (v_RESULT == 0 ? 1:0) );
    o_DATA_OUT.write( v_RESULT(31,0) );

}
