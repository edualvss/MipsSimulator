#include "PC_Control.h"

PC_Control::PC_Control(sc_module_name mn) : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor PC_Control" << std::endl;
#endif

    SC_METHOD(p_update);
    dont_initialize();
    sensitive << i_DVI << i_DVC << i_PC << i_IMED26 << i_JR << i_DATA;
}

void PC_Control::p_update() {
#ifdef DEBUG_METHODS
    std::cout << "PC_Control::p_update" << std::endl;
#endif

    sc_uint<32> v_PC_PLUS4;
    sc_uint<26> v_IMED26;
    sc_uint<32> v_OUT;

    v_PC_PLUS4 = i_PC.read() + 4;
    v_IMED26 = i_IMED26.read();
    if( i_JR.read() == 1 ) {
        v_OUT = i_DATA.read();
    } else {
        if( i_DVI.read() == 1 ) { // Unconditional
            v_OUT.range(31,28) = v_PC_PLUS4(31,28);
            sc_uint<28> v_SHIFT;
            v_SHIFT = v_IMED26 << 2;// Shift left 2 positions
            v_OUT.range(27,0) = v_SHIFT;
        } else {
            if( i_DVC.read() == 1 ) { // Conditional
                sc_uint<32> v_SIGNAL_EXTENDED;
                v_SIGNAL_EXTENDED.range(15,0) = v_IMED26(15,0);
                if( v_IMED26[15] == 1 ) {
                    v_SIGNAL_EXTENDED(31,16) = 0xFFFF;
                }
                v_SIGNAL_EXTENDED = v_SIGNAL_EXTENDED << 2;
                v_OUT = v_SIGNAL_EXTENDED + v_PC_PLUS4;
            } else {
                v_OUT = v_PC_PLUS4;
            }
        }
    }

    o_PC_PLUS4.write(v_PC_PLUS4);
    o_NEXT_PC.write(v_OUT);
}
