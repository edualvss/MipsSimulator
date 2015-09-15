/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : Mips
FILE   : MipsTop.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements a high-level Mips monocycle
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
--------------------------------------------------------------------------------
Last update on:
31/08/2015 - Eduardo Alves - Initial elaboration
--------------------------------------------------------------------------------
*/
#ifndef MIPSTOP
#define MIPSTOP

#include <systemc>

using namespace sc_core;
using namespace sc_dt;

// Forward declaration
class ProgramCounter;
class InstructionMemory;
class Control;
class Decoder;
class RegisterFile;
class Alu;
class DataMemory;
class PC_Control;
class Mux2x1_32bit;
class Mux2x1_5bit;
class SystemMonitor;

class Mips : public sc_module {
public:

    sc_in<bool> i_CLK;
    sc_in<bool> i_RST;
    // Sub-modules
    ProgramCounter* c_PC;
    PC_Control* c_PC_Control;
    InstructionMemory* c_InstructionMemory;
    Control* c_Control;
    Decoder* c_Decoder;
    RegisterFile* c_RegFile;
    DataMemory* c_DataMemory;
    Alu* c_ALU;
    Mux2x1_5bit* c_MuxInputRegFile;
    Mux2x1_5bit* c_MuxJalAddressRegFile;
    Mux2x1_32bit* c_MuxInputAluB;
    Mux2x1_32bit* c_MuxWriteBack;
    Mux2x1_32bit* c_MuxJalDataRegFile;
    SystemMonitor* c_systemMonitor;

    // Internal wires
    // PC_Control -> PC -> Instruction Memory  //
    sc_signal<sc_uint<32> > w_PC_IN;
    sc_signal<sc_uint<32> > w_PC_PLUS4;
    sc_signal<bool>         w_JR;
    sc_signal<sc_uint<32> > w_PC_OUT;
    // Instruction Memory -> Decoder ...
    sc_signal<sc_uint<32> > w_INSTRUCTION;
    // Decoder -> Control
    sc_signal<sc_uint<6> >  w_FUNCTION;
    sc_signal<sc_uint<16> > w_IMED16;
    sc_signal<sc_uint<26> > w_IMED26;
    sc_signal<sc_uint<6> >  w_OP;
    sc_signal<sc_uint<5> >  w_RD;
    sc_signal<sc_uint<5> >  w_RS;
    sc_signal<sc_uint<5> >  w_RT;
    sc_signal<sc_uint<5> >  w_SHAMT;
    // Control
    sc_signal<sc_uint<2> >  w_ALU_OP;
    sc_signal<bool>         w_REG_DST;
    sc_signal<bool>         w_JUMP;
    sc_signal<bool>         w_BRANCH;
    sc_signal<bool>         w_MEM_READ;
    sc_signal<bool>         w_MEM_TO_REG;
    sc_signal<bool>         w_MEM_WRITE;
    sc_signal<bool>         w_ALU_SRC;
    sc_signal<bool>         w_REG_WRITE;
    sc_signal<bool>         w_JAL;
    // Register File
    sc_signal<sc_uint<32> > w_DATA_WRITE_BACK;
    sc_signal<sc_uint<32> > w_DATA_IN_REG_FILE;
    sc_signal<sc_uint<5> >  w_REG_SELECTED;
    sc_signal<sc_uint<5> >  w_WR_ADDRESS_REG_FILE;
    sc_signal<sc_uint<32> > w_DATA_OUT_A_REG_FILE;
    sc_signal<sc_uint<32> > w_DATA_OUT_B_REG_FILE;
    sc_signal<sc_uint<5> >  w_ADDRESS_REGISTER_RA;
    // ALU
    sc_signal<sc_uint<32> > w_ALU_SIGN_EXTENDED;
    sc_signal<sc_uint<32> > w_ALU_B_INPUT;
    sc_signal<sc_uint<32> > w_ALU_OUT;
    sc_signal<bool>         w_ALU_ZERO;
    // Data Memory
    sc_signal<sc_uint<32> > w_DATA_MEM_OUT;
    // Branch AND
    sc_signal<bool>         w_BRANCH_RESULT;

    // Signal tracing
    sc_trace_file *tf;
    unsigned long long clockCount;

    // Module's processes
    void p_signExtend();
    void p_branch();
    void p_updateClockCount();

    SC_HAS_PROCESS(Mips);
    Mips(sc_module_name moduleName);
    virtual ~Mips();

};

#endif // MIPSTOP

