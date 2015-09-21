/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : Control
FILE   : Control.cpp
--------------------------------------------------------------------------------
*/

#include "Control.h"


Control::Control(sc_module_name mn) : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor Control" << std::endl;
#endif

    SC_METHOD(p_control);
    dont_initialize();
    sensitive << i_DATA_OP << i_DATA_FUNCTION;

}

////////////////////////////////////////////////////////////////////////////////
void Control::p_control(){
////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_METHODS
    std::cout << "Control::p_control" << std::endl;
#endif

    // Temporary variables for manipulation of bit vectors
    sc_uint<6> v_OPCODE;
    sc_uint<6> v_FUNCTION;
    sc_uint<2> v_DATA_ALU_OP;

    bool v_OP_R_FORMAT;
    bool v_OP_LW;
    bool v_OP_SW;
    bool v_OP_BEQ;
    bool v_OP_BNE;
    bool v_OP_J;
    bool v_OP_ADDI;
    bool v_OP_ANDI;
    bool v_OP_ORI;
    bool v_OP_XORI;
    bool v_OP_SLTI;
    bool v_OP_LUI;
    bool v_OP_JAL;
    bool v_FUNC_JR;

    v_OPCODE = i_DATA_OP.read();
    v_FUNCTION = i_DATA_FUNCTION.read();

    v_OP_R_FORMAT = (!v_OPCODE[5] and !v_OPCODE[4] and !v_OPCODE[3] and !v_OPCODE[2] and !v_OPCODE[1] and !v_OPCODE[0]);
    v_OP_LW       = (v_OPCODE[5] and !v_OPCODE[4] and !v_OPCODE[3] and !v_OPCODE[2] and v_OPCODE[1] and v_OPCODE[0]);
    v_OP_SW       = (v_OPCODE[5] and !v_OPCODE[4] and v_OPCODE[3] and !v_OPCODE[2] and v_OPCODE[1] and v_OPCODE[0]);
    v_OP_BEQ      = (!v_OPCODE[5] and !v_OPCODE[4] and !v_OPCODE[3] and v_OPCODE[2] and !v_OPCODE[1] and !v_OPCODE[0]);
    v_OP_BNE      = (!v_OPCODE[5] and !v_OPCODE[4] and !v_OPCODE[3] and v_OPCODE[2] and !v_OPCODE[1] and v_OPCODE[0]);
    v_OP_J        = (!v_OPCODE[5] and !v_OPCODE[4] and !v_OPCODE[3] and !v_OPCODE[2] and v_OPCODE[1] and !v_OPCODE[0]);
    v_OP_JAL      = (!v_OPCODE[5] and !v_OPCODE[4] and !v_OPCODE[3] and !v_OPCODE[2] and v_OPCODE[1] and v_OPCODE[0]);
    v_OP_ADDI     = (!v_OPCODE[5] and !v_OPCODE[4] and v_OPCODE[3] and !v_OPCODE[2] and !v_OPCODE[1] and !v_OPCODE[0]);
    v_OP_ANDI     = (!v_OPCODE[5] and !v_OPCODE[4] and v_OPCODE[3] and v_OPCODE[2] and !v_OPCODE[1] and !v_OPCODE[0]);
    v_OP_ORI      = (!v_OPCODE[5] and !v_OPCODE[4] and v_OPCODE[3] and v_OPCODE[2] and !v_OPCODE[1] and v_OPCODE[0]);
    v_OP_XORI     = (!v_OPCODE[5] and !v_OPCODE[4] and v_OPCODE[3] and v_OPCODE[2] and v_OPCODE[1] and !v_OPCODE[0]);
    v_OP_SLTI     = (!v_OPCODE[5] and !v_OPCODE[4] and v_OPCODE[3] and !v_OPCODE[2] and v_OPCODE[1] and !v_OPCODE[0]);
    v_OP_LUI      = (!v_OPCODE[5] and !v_OPCODE[4] and v_OPCODE[3] and v_OPCODE[2] and v_OPCODE[1] and v_OPCODE[0]);

    v_FUNC_JR     = (!v_FUNCTION[5] and !v_FUNCTION[4] and v_FUNCTION[3] and !v_FUNCTION[2] and !v_FUNCTION[1] and !v_FUNCTION[0]);

    o_DATA_REG_DST.write(v_OP_R_FORMAT);
    o_DATA_ALU_SRC.write(v_OP_LW or v_OP_SW or v_OP_ADDI or v_OP_LUI or v_OP_ORI
                         or v_OP_SLTI or v_OP_ANDI or v_OP_XORI);
    o_DATA_MEM_TO_REG.write(v_OP_LW);
    o_DATA_REG_WRITE.write(v_OP_R_FORMAT or v_OP_LW or v_OP_ADDI or v_OP_LUI or v_OP_ORI
                           or v_OP_SLTI or v_OP_ANDI or v_OP_XORI or v_OP_JAL);
    o_DATA_MEM_READ.write(v_OP_LW);
    o_DATA_MEM_WRITE.write(v_OP_SW);
    o_DATA_BRANCH.write(v_OP_BEQ or v_OP_BNE);
    o_DATA_DVI.write(v_OP_J or v_OP_JAL);
    o_DATA_JAL.write(v_OP_JAL);
    o_DATA_JR.write( v_OP_R_FORMAT and v_FUNC_JR );

    v_DATA_ALU_OP[1] = v_OP_R_FORMAT;
    v_DATA_ALU_OP[0] = v_OP_BEQ or v_OP_BNE;

    o_DATA_ALU_OP.write(v_DATA_ALU_OP);

}
