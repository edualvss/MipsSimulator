#include "MipsTop.h"

#include "Alu.h"
#include "Control.h"
#include "DataMemory.h"
#include "Decoder.h"
#include "Instruction.h"
#include "InstructionMemory.h"
#include "Mux2x1.h"
#include "PC_Control.h"
#include "ProgramCounter.h"
#include "RegisterFile.h"

Mips::Mips(sc_module_name mn, const char* instructionMem,const char* dataMem) : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor MIPS" << std::endl;
#endif

    SC_METHOD(p_signExtend);
    dont_initialize();
    sensitive << w_IMED16;

    SC_METHOD(p_branch);
    dont_initialize();
    sensitive << w_ALU_ZERO << w_BRANCH;

    ///////// Program Counter /////////
    c_PC = new ProgramCounter("PC");
    c_PC->i_CLK(i_CLK);
    c_PC->i_DATA_IN(w_PC_IN);
    c_PC->i_RST(i_RST);
    c_PC->o_DATA_OUT(w_PC_OUT);

    ///////// Instruction Memory /////////
    c_InstructionMemory = new InstructionMemory("InstructionMemory");
    c_InstructionMemory->initialize(instructionMem);
    c_InstructionMemory->i_DATA_ADDRESS(w_PC_OUT);
    c_InstructionMemory->o_DATA_INSTRUCTION(w_INSTRUCTION);

    ///////// Decoder /////////
    c_Decoder = new Decoder("InstructionDecoder");
    c_Decoder->i_DATA_IN(w_INSTRUCTION);
    c_Decoder->o_DATA_FUNCTION(w_FUNCTION);
    c_Decoder->o_DATA_IMED16(w_IMED16);
    c_Decoder->o_DATA_IMED26(w_IMED26);
    c_Decoder->o_DATA_OP(w_OP);
    c_Decoder->o_DATA_RD(w_RD);
    c_Decoder->o_DATA_RS(w_RS);
    c_Decoder->o_DATA_RT(w_RT);
    c_Decoder->o_DATA_SHAMT(w_SHAMT);

    ///////// Control /////////
    c_Control = new Control("Control");
    c_Control->i_DATA_OP(w_OP);
    c_Control->o_DATA_ALU_OP(w_ALU_OP);
    c_Control->o_DATA_ALU_SRC(w_ALU_SRC);
    c_Control->o_DATA_BRANCH(w_BRANCH);
    c_Control->o_DATA_DVI(w_JUMP);
    c_Control->o_DATA_MEM_READ(w_MEM_READ);
    c_Control->o_DATA_MEM_TO_REG(w_MEM_TO_REG);
    c_Control->o_DATA_MEM_WRITE(w_MEM_WRITE);
    c_Control->o_DATA_REG_DST(w_REG_DST);
    c_Control->o_DATA_REG_WRITE(w_REG_WRITE);

    ///////// Mux Register File Input /////////
    c_MuxInputRegFile = new Mux2x1_5bit("MuxRegisterSelect");
    c_MuxInputRegFile->i_DATA_A(w_RT);
    c_MuxInputRegFile->i_DATA_B(w_RD);
    c_MuxInputRegFile->i_SEL(w_REG_DST);
    c_MuxInputRegFile->o_DATA_OUT(w_REG_SELECTED);

    ///////// Register File /////////
    c_RegFile = new RegisterFile("RegisterFile");
    c_RegFile->i_DATA_IN(w_DATA_IN_REG_FILE);
    c_RegFile->i_RD_ADDRESS_0(w_RS);
    c_RegFile->i_RD_ADDRESS_1(w_RT);
    c_RegFile->i_WR_ADDRESS(w_REG_SELECTED);
    c_RegFile->i_WR_ENABLE(w_REG_WRITE);
    c_RegFile->o_DATA_OUT_0(w_DATA_OUT_A_REG_FILE);
    c_RegFile->o_DATA_OUT_1(w_DATA_OUT_B_REG_FILE);

    ///////// Mux ALU Input /////////
    c_MuxInputAluB = new Mux2x1_32bit("MuxALU_B_Input");
    c_MuxInputAluB->i_SEL(w_ALU_SRC);
    c_MuxInputAluB->i_DATA_A(w_DATA_OUT_B_REG_FILE);
    c_MuxInputAluB->i_DATA_B(w_ALU_SIGN_EXTENDED);
    c_MuxInputAluB->o_DATA_OUT(w_ALU_B_INPUT);

    ///////// ALU /////////
    c_ALU = new Alu("Alu");
    c_ALU->i_DATA_A(w_DATA_OUT_A_REG_FILE);
    c_ALU->i_DATA_B(w_ALU_B_INPUT);
    c_ALU->i_DATA_OP(w_ALU_OP);
    c_ALU->i_FUNCTION(w_FUNCTION);
    c_ALU->o_DATA_OUT(w_ALU_OUT);
    c_ALU->o_DATA_ZERO(w_ALU_ZERO);

    ///////// Data Memory /////////
    c_DataMemory = new DataMemory("DataMemory");
//    c_DataMemory->initialize(dataMem);
    c_DataMemory->i_ADDRESS(w_ALU_OUT);
    c_DataMemory->i_DATA_IN(w_DATA_OUT_B_REG_FILE);
    c_DataMemory->i_RD_ENABLE(w_MEM_READ);
    c_DataMemory->i_WR_ENABLE(w_MEM_WRITE);
    c_DataMemory->o_DATA_OUT(w_DATA_MEM_OUT);

    ///////// Mux Write Back /////////
    c_MuxWriteBack = new Mux2x1_32bit("MuxWriteBack");
    c_MuxWriteBack->i_SEL(w_MEM_TO_REG);
    c_MuxWriteBack->i_DATA_A(w_ALU_OUT);
    c_MuxWriteBack->i_DATA_B(w_DATA_MEM_OUT);
    c_MuxWriteBack->o_DATA_OUT(w_DATA_IN_REG_FILE);


    ///////// PC_Control /////////
    c_PC_Control = new PC_Control("PC_Update");
    c_PC_Control->i_DVC(w_BRANCH_RESULT);
    c_PC_Control->i_DVI(w_JUMP);
    c_PC_Control->i_IMED26(w_IMED26);
    c_PC_Control->i_PC(w_PC_OUT);
    c_PC_Control->o_NEXT_PC(w_PC_IN);

    // Signal tracing
    sc_trace_file *tf = sc_create_vcd_trace_file("waves");
    sc_trace(tf,i_CLK,"CLK");
    sc_trace(tf,i_RST,"RST");
    sc_trace(tf,w_PC_IN,"NEXT_PC");
    sc_trace(tf,w_PC_OUT,"PC_OUT" );
    sc_trace(tf,w_INSTRUCTION,"INSTRUCTION");
    sc_trace(tf,w_FUNCTION,"FUNCTION");
    sc_trace(tf,w_IMED16,"IMED16");
    sc_trace(tf,w_IMED26,"IMED26");
    sc_trace(tf,w_OP,"OP");
    sc_trace(tf,w_RD,"RD");
    sc_trace(tf,w_RS,"RS");
    sc_trace(tf,w_RT,"RT");
    sc_trace(tf,w_SHAMT,"SHAMT");
    sc_trace(tf,w_DATA_OUT_A_REG_FILE,"DATA_A_REG_FILE");
    sc_trace(tf,w_DATA_OUT_B_REG_FILE,"DATA_B_REG_FILE");
    sc_trace(tf,w_ALU_OP,"ALU_OP");
    sc_trace(tf,w_ALU_OUT,"ALU_OUT");
    sc_trace(tf,w_DATA_MEM_OUT,"DATA_MEM_OUT");
    sc_trace(tf,w_ALU_SRC,"ALU_SRC");
    sc_trace(tf,w_ALU_ZERO,"ALU_ZERO");
    sc_trace(tf,w_BRANCH,"DVC");
    sc_trace(tf,w_BRANCH_RESULT,"BRANCH");
    sc_trace(tf,w_JUMP,"DVI");
    sc_trace(tf,w_MEM_READ,"MEM_READ");
    sc_trace(tf,w_MEM_TO_REG,"MEM_TO_REG");
    sc_trace(tf,w_MEM_WRITE,"MEM_WRITE");
    sc_trace(tf,w_REG_DST,"REG_DST");
    sc_trace(tf,w_REG_WRITE,"REG_WRITE");
    sc_trace(tf,w_DATA_IN_REG_FILE,"DATA_IN_REG_FILE");
    sc_trace(tf,w_REG_SELECTED,"ADDR_WR_REG_FILE");
    sc_trace(tf,w_RS,"ADDR_RD_1_REG_FILE");
    sc_trace(tf,w_RT,"ADDR_RD_2_REG_FILE");
    sc_trace(tf,w_DATA_OUT_A_REG_FILE,"ALU_IN_A");
    sc_trace(tf,w_ALU_B_INPUT,"ALU_IN_B");

}

void Mips::p_signExtend() {
#ifdef DEBUG_METHODS
    std::cout << "Mips::p_signExtend" << std::endl;
#endif

    sc_uint<32> v_SIGN_EXTENDED;
    v_SIGN_EXTENDED.range(15,0) = w_IMED16.read();
    v_SIGN_EXTENDED(31,16) = v_SIGN_EXTENDED[15];

    w_ALU_SIGN_EXTENDED.write(v_SIGN_EXTENDED);
}

void Mips::p_branch() {
#ifdef DEBUG_METHODS
    std::cout << "Mips::p_branch" << std::endl;
#endif

    w_BRANCH_RESULT.write( (w_BRANCH.read() and w_ALU_ZERO.read() ) );

}


Mips::~Mips() {
#ifdef DEBUG_METHODS
    std::cout << "Destructor Mips" << std::endl;
#endif

    sc_close_vcd_trace_file(tf);
    delete c_PC;
    delete c_PC_Control;
    delete c_InstructionMemory;
    delete c_Control;
    delete c_Decoder;
    delete c_RegFile;
    delete c_ALU;
    delete c_MuxInputRegFile;
    delete c_MuxInputAluB;
    delete c_MuxWriteBack;
}
