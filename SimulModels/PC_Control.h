#ifndef PC_CONTROL_H
#define PC_CONTROL_H

#include <systemc>

using namespace sc_core;
using namespace sc_dt;

class PC_Control : public sc_module {
public:

    // Interface
    sc_in<bool>          i_DVI;     // Input Inconditional Jump
    sc_in<bool>          i_DVC;     // Input Conditional Jump
    sc_in<bool>          i_JR;      // Input Jump Register
    sc_in<sc_uint<32> >  i_PC;      // Input data PC
    sc_in<sc_uint<26> >  i_IMED26;  // Instruction [25..0]
    sc_in<sc_uint<32> >  i_DATA;    // Input binded with output A from Register File to jump register
    sc_out<sc_uint<32> > o_NEXT_PC; // Output
    sc_out<sc_uint<32> > o_PC_PLUS4;// PC + 4

    // Module's process
    void p_update();

    SC_HAS_PROCESS(PC_Control);
    PC_Control(sc_module_name);
};

#endif // PC_CONTROL_H
