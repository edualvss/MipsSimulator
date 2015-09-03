#include "PC_Control.h"

PC_Control::PC_Control(sc_module_name mn) : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor PC_Control" << std::endl;
#endif

    SC_METHOD(p_update);
    dont_initialize();
    sensitive << i_DVI << i_DVC << i_PC << i_IMED26;
}

void PC_Control::p_update() {
#ifdef DEBUG_METHODS
    std::cout << "PC_Control::p_update" << std::endl;
#endif

    sc_uint<32> v_OUT;
    sc_uint<32> v_NEXT_PC;
    sc_uint<26> v_IMED26;

    v_NEXT_PC = i_PC.read();
    v_NEXT_PC += 4;
    v_IMED26 = i_IMED26.read();

    if( i_DVI.read() == 1 ) { // Unconditional
        v_OUT.range(31,28) = v_NEXT_PC(31,28);
        v_OUT.range(27,0) = v_IMED26 << 2; // Shift left 2 positions
    } else {
        if( i_DVC.read() == 1 ) { // Conditional
            sc_uint<32> v_SIGNAL_EXTENDED;
            v_SIGNAL_EXTENDED.range(15,0) = v_IMED26(15,0);
            v_SIGNAL_EXTENDED(31,16) = v_IMED26[15];
            v_SIGNAL_EXTENDED = v_SIGNAL_EXTENDED << 2;
            v_OUT = v_SIGNAL_EXTENDED + v_NEXT_PC;
        } else { // Normal
            v_OUT = v_NEXT_PC;
        }
    }

    o_NEXT_PC.write(v_OUT);
}
