#include "SimulModels/Testbench.h"
#include "SimulModels/MipsTop.h"

#include "SimulModels/Decoder.h"
#include "SimulModels/Instruction.h"
#include "SimulModels/RegisterFile.h"
#include "SimulModels/InstructionMemory.h"
#include "SimulModels/DataMemory.h"

#include <time.h>

int sc_main(int , char**) {
#ifdef DEBUG_METHODS
    cout << "Main" << std::endl;
#endif

    int              clockTime = 10;
    sc_clock         w_CLK("CLK",clockTime,SC_NS);      // System clock with 10 ns period
    sc_signal<bool>  w_RST;

    Testbench *tb = new Testbench("Tester");
    tb->i_CLK(w_CLK);
    tb->o_RST(w_RST);

    Mips* mips = new Mips("MipsMono");
    mips->c_InstructionMemory->initialize("tests/InsBubbleSort.txt");
    mips->c_DataMemory->initialize("tests/DataBubbleSort.txt");
    mips->c_DataMemory->debug();
    mips->i_CLK(w_CLK);
    mips->i_RST(w_RST);

    clock_t t0 = clock();
    sc_start();
    clock_t t1 = clock();

    mips->c_RegFile->debug();
/*
    std::cout << "\nInicio!" << std::endl;
    sc_start(SC_ZERO_TIME);
    sc_start(clockTime/2,SC_NS);
    mips->c_RegFile->debug();

    int numPassos = 60;
    for( int i = 1; i <= numPassos; i++  ) {
        std::cout << "\n\nCiclo " << i << "!" << std::endl;
        sc_start(clockTime,SC_NS);
        mips->c_RegFile->debug();
        // Antes, 1, 2ª escrita: ciclos 30,31,32

    }
*/
    mips->c_DataMemory->debug();

    std::cout << "\nTinicial: "<<t0<<", Tfinal: "<<t1<<", Tduracao: " << (t1-t0) << std::endl;

    delete tb;
    delete mips;

    std::cout << std::endl;
    return 0;
}

