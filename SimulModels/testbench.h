#ifndef TESTBENCH
#define TESTBENCH

#include <systemc.h>

SC_MODULE(Testbench) {

    sc_in_clk            i_CLK;
    sc_out<bool>         o_RST;

    void process() {
#ifdef DEBUG_METHODS
    cout << "Main" << std::endl;
#endif

        o_RST.write(1);
        wait();
        o_RST.write(0);
        wait();
        wait();
        wait();
        wait();
        wait();
        wait();
//        wait();
//        wait();
//        wait();

        sc_stop();
    }

    SC_CTOR(Testbench) {
        SC_CTHREAD(process,i_CLK.pos());
        sensitive << i_CLK;
    }
};

#endif // TESTBENCH

