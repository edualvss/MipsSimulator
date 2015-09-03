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
    sensitive << i_DATA_OP << i_FUNCTION << i_OPCODE;

    SC_METHOD(p_operation);
    dont_initialize();
    sensitive << w_ALU_OP << i_DATA_A << i_DATA_B << i_SHAMT;

}

void Alu::p_updateOp() {
#ifdef DEBUG_METHODS
    std::cout << "Alu::p_updateOp" << std::endl;
#endif

    sc_uint<6> v_FUNCTION;
    sc_uint<2> v_ALU_OP;
    sc_uint<4> v_OUT;
    sc_uint<6> v_OP;

    v_OP = i_OPCODE.read();
    v_ALU_OP = i_DATA_OP.read();
    v_FUNCTION = i_FUNCTION.read();

    // Original control
    v_OUT[3] = 0;
    v_OUT[2] = v_ALU_OP[0] or (v_ALU_OP[1] and v_FUNCTION[1]);
    v_OUT[1] = !v_ALU_OP[1] or !v_FUNCTION[2];
    v_OUT[0] = v_ALU_OP[1] and (v_FUNCTION[3] or v_FUNCTION[0]);

    // Decoding operations not present in original control
    switch( v_OP ) {
        case 0b000000:{ // R-format
            switch(v_FUNCTION) {
                case 0b000000: // sll
                    v_OUT = 9;
                    break;
                case 0b000010: // srl
                    v_OUT = 10;
                    break;
                case 0b100110: // xor
                    v_OUT = 8;
                    break;
                case 0b100111: // nor
                    v_OUT = 12;
                    break;
            }
            break;
        } // End R-format
        case 0b001100: // ANDI
            v_OUT = 0;
            break;
        case 0b001101: // ORI
            v_OUT = 1;
            break;
        case 0b001010: // SLTI
            v_OUT = 7;
            break;
        case 0b001110: // XORI
            v_OUT = 8;
            break;
        default: break;
    }

    w_ALU_OP.write(v_OUT);
}

void Alu::p_operation() {
#ifdef DEBUG_METHODS
    std::cout << "Alu::p_operation" << std::endl;
#endif

    sc_uint<4>  v_ALU_OP;
    sc_uint<32> v_A;
    sc_uint<32> v_B;
    sc_uint<33> v_RESULT;
    sc_uint<5>  v_SHAMT;

    v_A = i_DATA_A.read();
    v_B = i_DATA_B.read();
    v_ALU_OP = w_ALU_OP.read();
    v_SHAMT = i_SHAMT.read();

    switch(v_ALU_OP) {
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
        case 8: // Added for xor & xori
            v_RESULT = v_A xor v_B;
            break;
        case 9: // Added for sll
            v_RESULT = v_B << v_SHAMT;
            break;
        case 10: // Added for srl
            v_RESULT = v_B >> v_SHAMT;
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
