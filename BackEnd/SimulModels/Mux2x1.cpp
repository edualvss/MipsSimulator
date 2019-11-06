#include "Mux2x1.h"

Mux2x1_32bit::Mux2x1_32bit(sc_module_name mn) : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor Mux2x1_32bit" << std::endl;
#endif

    SC_METHOD(p_update);
    dont_initialize();
    sensitive << i_SEL << i_DATA_A << i_DATA_B;

}

///////////////////////////////////////////////////////////////////
void Mux2x1_32bit::p_update() {
///////////////////////////////////////////////////////////////////
#ifdef DEBUG_METHODS
    std::cout << "Mux2x1_32bit::p_update" << std::endl;
#endif

    if( i_SEL.read() == 0 ) {
        o_DATA_OUT.write(i_DATA_A.read());
    } else {
        o_DATA_OUT.write(i_DATA_B.read());
    }
}

Mux2x1_5bit::Mux2x1_5bit(sc_module_name mn) : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor Mux2x1_5bit" << std::endl;
#endif

    SC_METHOD(p_update);
    dont_initialize();
    sensitive << i_SEL << i_DATA_A << i_DATA_B;

}

///////////////////////////////////////////////////////////////////
void Mux2x1_5bit::p_update() {
///////////////////////////////////////////////////////////////////
#ifdef DEBUG_METHODS
    std::cout << "Mux2x1_5bit::p_update" << std::endl;
#endif

    if( i_SEL.read() == 0 ) {
        o_DATA_OUT.write(i_DATA_A.read());
    } else {
        o_DATA_OUT.write(i_DATA_B.read());
    }
}
