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
    sensitive << i_DATA_OP;

}

////////////////////////////////////////////////////////////////////////////////
void Control::p_control(){
////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_METHODS
    std::cout << "Control::p_control" << std::endl;
#endif

    // Temporary variables for manipulation of bit vectors
    sc_uint<6> v_DATA_IN;
    sc_uint<2> v_DATA_ALU_OP;

    bool v_DATA_R_FORMAT;
    bool v_DATA_LW;
    bool v_DATA_SW;
    bool v_DATA_BEQ;
    bool v_DATA_BNE;
    bool v_DATA_DVI;
    bool v_DATA_ADDI;
    bool v_DATA_LUI;
    bool v_DATA_ORI;

    v_DATA_IN = i_DATA_OP.read();

    v_DATA_R_FORMAT = (!v_DATA_IN[5] and !v_DATA_IN[4] and !v_DATA_IN[3] and !v_DATA_IN[2] and !v_DATA_IN[1] and !v_DATA_IN[0]);
    v_DATA_LW = (v_DATA_IN[5] and !v_DATA_IN[4] and !v_DATA_IN[3] and !v_DATA_IN[2] and v_DATA_IN[1] and v_DATA_IN[0]);
    v_DATA_SW = (v_DATA_IN[5] and !v_DATA_IN[4] and v_DATA_IN[3] and !v_DATA_IN[2] and v_DATA_IN[1] and v_DATA_IN[0]);
    v_DATA_BEQ = (!v_DATA_IN[5] and !v_DATA_IN[4] and !v_DATA_IN[3] and v_DATA_IN[2] and !v_DATA_IN[1] and !v_DATA_IN[0]);
    v_DATA_BNE = (!v_DATA_IN[5] and !v_DATA_IN[4] and !v_DATA_IN[3] and v_DATA_IN[2] and !v_DATA_IN[1] and v_DATA_IN[0]);
    v_DATA_DVI = (!v_DATA_IN[5] and !v_DATA_IN[4] and !v_DATA_IN[3] and !v_DATA_IN[2] and v_DATA_IN[1] and !v_DATA_IN[0]);
    v_DATA_ADDI = (!v_DATA_IN[5] and !v_DATA_IN[4] and v_DATA_IN[3] and !v_DATA_IN[2] and !v_DATA_IN[1] and !v_DATA_IN[0]);
    v_DATA_LUI = (!v_DATA_IN[5] and !v_DATA_IN[4] and v_DATA_IN[3] and v_DATA_IN[2] and v_DATA_IN[1] and v_DATA_IN[0]);
    v_DATA_ORI = (!v_DATA_IN[5] and !v_DATA_IN[4] and v_DATA_IN[3] and v_DATA_IN[2] and !v_DATA_IN[1] and v_DATA_IN[0]);

    o_DATA_REG_DST.write(v_DATA_R_FORMAT);
    o_DATA_ALU_SRC.write(v_DATA_LW or v_DATA_SW or v_DATA_ADDI or v_DATA_LUI or v_DATA_ORI);
    o_DATA_MEM_TO_REG.write(v_DATA_LW);
    o_DATA_REG_WRITE.write(v_DATA_R_FORMAT or v_DATA_LW or v_DATA_ADDI or v_DATA_LUI or v_DATA_ORI);
    o_DATA_MEM_READ.write(v_DATA_LW);
    o_DATA_MEM_WRITE.write(v_DATA_SW);
    o_DATA_BRANCH.write(v_DATA_BEQ or v_DATA_BNE);
    o_DATA_DVI.write(v_DATA_DVI);

    v_DATA_ALU_OP[1] = v_DATA_R_FORMAT;
    v_DATA_ALU_OP[0] = v_DATA_BEQ or v_DATA_BNE;

    o_DATA_ALU_OP.write(v_DATA_ALU_OP);

}
