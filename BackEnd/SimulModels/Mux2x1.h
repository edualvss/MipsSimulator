#ifndef MUX2X1_H
#define MUX2X1_H

#include <systemc>

using namespace sc_core;
using namespace sc_dt;

class Mux2x1_32bit : public sc_module {
public:

    // Interface
    sc_in<bool>          i_SEL;       // Selector
    sc_in<sc_uint<32> >  i_DATA_A;    // Input A
    sc_in<sc_uint<32> >  i_DATA_B;    // Input B
    sc_out<sc_uint<32> > o_DATA_OUT;  // Output

    // Module's process
    void p_update();

    SC_HAS_PROCESS(Mux2x1_32bit);
    Mux2x1_32bit(sc_module_name);
};

class Mux2x1_5bit : public sc_module {
public:

    // Interface
    sc_in<bool>         i_SEL;       // Selector
    sc_in<sc_uint<5> >  i_DATA_A;    // Input A
    sc_in<sc_uint<5> >  i_DATA_B;    // Input B
    sc_out<sc_uint<5> > o_DATA_OUT;  // Output

    // Module's process
    void p_update();

    SC_HAS_PROCESS(Mux2x1_5bit);
    Mux2x1_5bit(sc_module_name);
};

#endif // MUX2X1_H
