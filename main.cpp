#include "SimulModels/Testbench.h"
#include "SimulModels/MipsTop.h"

#include "SimulModels/Decoder.h"
#include "SimulModels/Instruction.h"
#include "SimulModels/RegisterFile.h"
#include "SimulModels/InstructionMemory.h"
#include "SimulModels/DataMemory.h"

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
    mips->c_DataMemory->initialize("tests/DataMinimal.txt");
    mips->c_DataMemory->debug();
    mips->i_CLK(w_CLK);
    mips->i_RST(w_RST);

//    sc_start();
//    mips->c_RegFile->debug();

    std::cout << "\nInicio!" << std::endl;
    sc_start(SC_ZERO_TIME);
    sc_start(clockTime/2,SC_NS);
    mips->c_RegFile->debug();

    int numPassos = 56;
    for( int i = 1; i <= numPassos; i++  ) {
        std::cout << "\n\nCiclo " << i << "!" << std::endl;
        sc_start(clockTime,SC_NS);
        mips->c_RegFile->debug();
    }

    mips->c_DataMemory->debug();

    delete tb;
    delete mips;

    std::cout << std::endl;
    return 0;
}

