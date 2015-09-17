#include "MipsTop.h"

#include "Alu.h"
#include "Control.h"
#include "DataMemory.h"
#include "Decoder.h"
#include "InstructionMemory.h"
#include "Mux2x1.h"
#include "PC_Control.h"
#include "ProgramCounter.h"
#include "RegisterFile.h"
#include "SystemMonitor.h"

Mips::Mips(sc_module_name mn) : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor MIPS" << std::endl;
#endif

    clockCount = 0;
    w_ADDRESS_REGISTER_RA.write(31); // $ra

    SC_METHOD(p_updateClockCount);
    dont_initialize();
    sensitive << i_CLK.pos();

    SC_METHOD(p_signExtend);
    dont_initialize();
    sensitive << w_IMED16;

    SC_METHOD(p_branch);
    dont_initialize();
    sensitive << w_ALU_ZERO << w_BRANCH;

    ///////// Instruction Memory /////////
    c_InstructionMemory = new InstructionMemory("InstructionMemory");
    c_InstructionMemory->i_DATA_ADDRESS(w_PC_OUT);
    c_InstructionMemory->o_DATA_INSTRUCTION(w_INSTRUCTION);

    ///////// Program Counter /////////
    c_PC = new ProgramCounter("PC",&c_InstructionMemory->lastAddress);
    c_PC->i_CLK(i_CLK);
    c_PC->i_DATA_IN(w_PC_IN);
    c_PC->i_RST(i_RST);
    c_PC->o_DATA_OUT(w_PC_OUT);

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
    c_Control->i_DATA_FUNCTION(w_FUNCTION);
    c_Control->o_DATA_ALU_OP(w_ALU_OP);
    c_Control->o_DATA_ALU_SRC(w_ALU_SRC);
    c_Control->o_DATA_BRANCH(w_BRANCH);
    c_Control->o_DATA_DVI(w_JUMP);
    c_Control->o_DATA_MEM_READ(w_MEM_READ);
    c_Control->o_DATA_MEM_TO_REG(w_MEM_TO_REG);
    c_Control->o_DATA_MEM_WRITE(w_MEM_WRITE);
    c_Control->o_DATA_REG_DST(w_REG_DST);
    c_Control->o_DATA_REG_WRITE(w_REG_WRITE);
    c_Control->o_DATA_JAL(w_JAL);
    c_Control->o_DATA_JR(w_JR);

    ///////// Mux Register File Input /////////
    c_MuxInputRegFile = new Mux2x1_5bit("MuxRegisterSelect");
    c_MuxInputRegFile->i_DATA_A(w_RT);
    c_MuxInputRegFile->i_DATA_B(w_RD);
    c_MuxInputRegFile->i_SEL(w_REG_DST);
    c_MuxInputRegFile->o_DATA_OUT(w_REG_SELECTED);

    ///////// Mux Register File Jal Write Address Select /////////
    c_MuxJalAddressRegFile = new Mux2x1_5bit("MuxJalAddress");
    c_MuxJalAddressRegFile->i_SEL(w_JAL);
    c_MuxJalAddressRegFile->i_DATA_A(w_REG_SELECTED);
    c_MuxJalAddressRegFile->i_DATA_B(w_ADDRESS_REGISTER_RA);
    c_MuxJalAddressRegFile->o_DATA_OUT(w_WR_ADDRESS_REG_FILE);

    ///////// Mux Register File Jal DATA IN Select /////////
    c_MuxJalDataRegFile = new Mux2x1_32bit("MuxJalData");
    c_MuxJalDataRegFile->i_SEL(w_JAL);
    c_MuxJalDataRegFile->i_DATA_A(w_DATA_WRITE_BACK);
    c_MuxJalDataRegFile->i_DATA_B( w_PC_PLUS4 );
    c_MuxJalDataRegFile->o_DATA_OUT(w_DATA_IN_REG_FILE);

    ///////// Register File /////////
    c_RegFile = new RegisterFile("RegisterFile");
    c_RegFile->i_CLK(i_CLK);
    c_RegFile->i_DATA_IN(w_DATA_IN_REG_FILE);
    c_RegFile->i_RD_ADDRESS_0(w_RS);
    c_RegFile->i_RD_ADDRESS_1(w_RT);
    c_RegFile->i_WR_ADDRESS(w_WR_ADDRESS_REG_FILE);
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
    c_ALU->i_OPCODE(w_OP);
    c_ALU->i_SHAMT(w_SHAMT);
    c_ALU->i_DATA_A(w_DATA_OUT_A_REG_FILE);
    c_ALU->i_DATA_B(w_ALU_B_INPUT);
    c_ALU->i_DATA_OP(w_ALU_OP);
    c_ALU->i_FUNCTION(w_FUNCTION);
    c_ALU->o_DATA_OUT(w_ALU_OUT);
    c_ALU->o_DATA_ZERO(w_ALU_ZERO);

    ///////// Data Memory /////////
    c_DataMemory = new DataMemory("DataMemory");
    c_DataMemory->i_CLK(i_CLK);
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
    c_MuxWriteBack->o_DATA_OUT(w_DATA_WRITE_BACK);


    ///////// PC_Control /////////
    c_PC_Control = new PC_Control("PC_Update");
    c_PC_Control->i_DVI(w_JUMP);
    c_PC_Control->i_DVC(w_BRANCH_RESULT);
    c_PC_Control->i_IMED26(w_IMED26);
    c_PC_Control->i_PC(w_PC_OUT);
    c_PC_Control->i_JR(w_JR);
    c_PC_Control->i_DATA(w_DATA_OUT_A_REG_FILE);
    c_PC_Control->o_PC_PLUS4(w_PC_PLUS4);
    c_PC_Control->o_NEXT_PC(w_PC_IN);

    ///////////////// Monitor //////////////////
    /// Save the system status in each cycle ///
    /// ////////////////////////////////////////
    c_systemMonitor = new SystemMonitor("Monitor",c_RegFile,c_DataMemory,&clockCount);
    c_systemMonitor->i_CLK(i_CLK);
    c_systemMonitor->i_ALU_SRC(w_ALU_SRC);
    c_systemMonitor->i_BRANCH(w_BRANCH_RESULT);
    c_systemMonitor->i_CURRENT_PC(w_PC_OUT);
    c_systemMonitor->i_DVC(w_BRANCH);
    c_systemMonitor->i_DVI(w_JUMP);
    c_systemMonitor->i_INSTRUCTION(w_INSTRUCTION);
    c_systemMonitor->i_JAL(w_JAL);
    c_systemMonitor->i_JR(w_JR);
    c_systemMonitor->i_MEM_RD(w_MEM_READ);
    c_systemMonitor->i_MEM_TO_REG(w_MEM_TO_REG);
    c_systemMonitor->i_MEM_WR(w_MEM_WRITE);
    c_systemMonitor->i_NEXT_PC(w_PC_IN);
    c_systemMonitor->i_REG_DST(w_REG_DST);
    c_systemMonitor->i_REG_WRITE(w_REG_WRITE);
    c_systemMonitor->i_rf_DATA_IN(w_DATA_IN_REG_FILE);
    c_systemMonitor->i_rf_DATA_OUT_0(w_DATA_OUT_A_REG_FILE);
    c_systemMonitor->i_rf_DATA_OUT_1(w_DATA_OUT_B_REG_FILE);
    c_systemMonitor->i_rf_RD_ADDRESS_0(w_RS);
    c_systemMonitor->i_rf_RD_ADDRESS_1(w_RT);
    c_systemMonitor->i_rf_WR_ADDRESS(w_WR_ADDRESS_REG_FILE);
    c_systemMonitor->i_ula_IN_0(w_DATA_OUT_A_REG_FILE);
    c_systemMonitor->i_ula_IN_1(w_ALU_B_INPUT);
    c_systemMonitor->i_ula_OP(w_ALU_OP);
    c_systemMonitor->i_ula_OUT(w_ALU_OUT);
    c_systemMonitor->i_ula_ZERO(w_ALU_ZERO);

    // Signal tracing
    sc_trace_file *tf = sc_create_vcd_trace_file("waves");
    sc_trace(tf,i_CLK,"CLK");
    sc_trace(tf,clockCount,"CLK_Count");
    sc_trace(tf,i_RST,"RST");
    sc_trace(tf,w_PC_IN,"NEXT_PC");
    sc_trace(tf,w_PC_OUT,"PC_OUT" );
    sc_trace(tf,w_INSTRUCTION,"INSTRUCTION");
    sc_trace(tf,w_IMED16,"IMED16");
    sc_trace(tf,w_IMED26,"IMED26");
    sc_trace(tf,w_OP,"OP");
    sc_trace(tf,w_RD,"RD");
    sc_trace(tf,w_RS,"RS");
    sc_trace(tf,w_RT,"RT");
    sc_trace(tf,w_SHAMT,"SHAMT");
    sc_trace(tf,w_FUNCTION,"FUNCTION");
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
    sc_trace(tf,w_JAL,"JAL");
    sc_trace(tf,w_JR,"JR");
    sc_trace(tf,w_DATA_IN_REG_FILE,"DATA_IN_REG_FILE");
    sc_trace(tf,w_WR_ADDRESS_REG_FILE,"ADDR_WR_REG_FILE");
    sc_trace(tf,w_REG_SELECTED,"WR_REG_SELECTED");
    sc_trace(tf,w_RS,"ADDR_RD_1_REG_FILE(RS)");
    sc_trace(tf,w_RT,"ADDR_RD_2_REG_FILE(RT)");
    sc_trace(tf,w_DATA_OUT_A_REG_FILE,"ALU_IN_A");
    sc_trace(tf,w_ALU_B_INPUT,"ALU_IN_B");

}

void Mips::p_updateClockCount() {
#ifdef DEBUG_METHODS
    std::cout << "Mips::p_updateClockCount" << std::endl;
#endif
    clockCount++;
}

void Mips::p_signExtend() {
#ifdef DEBUG_METHODS
    std::cout << "Mips::p_signExtend" << std::endl;
#endif

    sc_uint<32> v_SIGN_EXTENDED = 0;
    v_SIGN_EXTENDED.range(15,0) = w_IMED16.read();

    if( v_SIGN_EXTENDED[15] == 1 ) {
        v_SIGN_EXTENDED(31,16) = 0xFFFF;
    }

    sc_uint<6> v_OP;
    v_OP = w_OP.read();

    if(v_OP == 0b001111) { // lui - Load Upper Immediate
        v_SIGN_EXTENDED = v_SIGN_EXTENDED << 16;
    }

    w_ALU_SIGN_EXTENDED.write(v_SIGN_EXTENDED);
}

void Mips::p_branch() {
#ifdef DEBUG_METHODS
    std::cout << "Mips::p_branch" << std::endl;
#endif

    sc_uint<6> v_OP;
    v_OP = w_OP.read();

    bool result;

    if( v_OP == 0b000101 ) { // bne - Branch on not equal
        result = (w_BRANCH.read() and !w_ALU_ZERO.read() );
    } else { // beq - Branch on equal
        result = (w_BRANCH.read() and w_ALU_ZERO.read() );

    }
    w_BRANCH_RESULT.write( result );
}

Mips::~Mips() {
#ifdef DEBUG_METHODS
    std::cout << "Destructor Mips" << std::endl;
#endif

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
    delete c_MuxJalAddressRegFile;
    delete c_MuxJalDataRegFile;
    delete c_systemMonitor;
}
