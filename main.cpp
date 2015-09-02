#include "SimulModels/testbench.h"
#include "SimulModels/MipsTop.h"

#include "SimulModels/Decoder.h"
#include "SimulModels/Instruction.h"
#include "SimulModels/RegisterFile.h"

int sc_main(int , char**) {
#ifdef DEBUG_METHODS
    cout << "Main" << std::endl;
#endif

    int clockTime = 10;
    sc_clock w_CLK("CLK",clockTime,SC_NS);      // System clock with 10 ns period
    sc_signal<bool>         w_RST;

    Testbench *tb = new Testbench("Tester");
    tb->i_CLK(w_CLK);
    tb->o_RST(w_RST);

    Mips* mips = new Mips("MipsMono","tests/Ex1.txt","");
    mips->i_CLK(w_CLK);
    mips->i_RST(w_RST);

/*
    // Start the simulation
    sc_start(10,SC_NS);
    Instruction* ins = mips->c_Decoder->getInstructionDecoded();
    ins->debug();
    delete ins;
    sc_start(10,SC_NS);
    ins = mips->c_Decoder->getInstructionDecoded();
    ins->debug();
    delete ins;
    sc_start(10,SC_NS);
    ins = mips->c_Decoder->getInstructionDecoded();
    ins->debug();
    delete ins;
    sc_start(10,SC_NS);
    ins = mips->c_Decoder->getInstructionDecoded();
    ins->debug();
    delete ins;
    sc_start(10,SC_NS);
    ins = mips->c_Decoder->getInstructionDecoded();
    ins->debug();
    delete ins;

*/
    std::cout << "Inicio!" << std::endl;
    sc_start(clockTime,SC_NS);
    mips->c_RegFile->debug();

    std::cout << "\n\nCiclo 1!" << std::endl;
    sc_start(clockTime,SC_NS);
    mips->c_RegFile->debug();

    std::cout << "\n\nCiclo 2!" << std::endl;
    sc_start(clockTime,SC_NS);
    mips->c_RegFile->debug();

    std::cout << "\n\nCiclo 3!" << std::endl;
    sc_start(clockTime,SC_NS);
    mips->c_RegFile->debug();

    std::cout << "\n\nCiclo 4!" << std::endl;
    sc_start(clockTime,SC_NS);
    mips->c_RegFile->debug();

//    std::cout << "\n\nCiclo 5!" << std::endl;
//    sc_start(clockTime,SC_NS);
//    mips->c_RegFile->debug();

//    std::cout << "\n\nCiclo 6!" << std::endl;
//    sc_start(clockTime,SC_NS);
//    mips->c_RegFile->debug();


    delete tb;
    delete mips;

    return 0;
}

