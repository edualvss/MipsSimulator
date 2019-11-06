#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <systemc>
#include <map>

using namespace sc_core;
using namespace sc_dt;

class CycleStatus;
class RegisterFile;
class DataMemory;

class SystemMonitor : public sc_module {

    std::vector<CycleStatus* >* steps;
    RegisterFile* regFile;
    DataMemory* dataMemory;
    unsigned long long int* cycleCount;

    void copyDataMemory(std::map<unsigned int, sc_uint<32> > *src,CycleStatus* step);

public:

    // Interface
    sc_in<bool>               i_CLK;
    sc_in<sc_uint<32> >       i_CURRENT_PC;
    sc_in<sc_uint<32> >       i_NEXT_PC;
    sc_in<sc_uint<32> >       i_INSTRUCTION;
    sc_in<sc_uint<5> >        i_rf_RD_ADDRESS_0;
    sc_in<sc_uint<5> >        i_rf_RD_ADDRESS_1;
    sc_in<sc_uint<5> >        i_rf_WR_ADDRESS;
    sc_in<sc_uint<32> >       i_rf_DATA_IN;
    sc_in<sc_uint<32> >       i_rf_DATA_OUT_0;
    sc_in<sc_uint<32> >       i_rf_DATA_OUT_1;
    sc_in<sc_uint<2> >        i_ula_OP;
    sc_in<sc_uint<32> >       i_ula_IN_0;
    sc_in<sc_uint<32> >       i_ula_IN_1;
    sc_in<bool>               i_ula_ZERO;
    sc_in<sc_uint<32> >       i_ula_OUT;
    sc_in<bool>               i_DVC;
    sc_in<bool>               i_BRANCH;
    sc_in<bool>               i_JR;
    sc_in<bool>               i_JAL;
    sc_in<bool>               i_DVI;
    sc_in<bool>               i_REG_DST;
    sc_in<bool>               i_MEM_RD;
    sc_in<bool>               i_MEM_TO_REG;
    sc_in<bool>               i_MEM_WR;
    sc_in<bool>               i_ALU_SRC;
    sc_in<bool>               i_REG_WRITE;

    std::vector<CycleStatus *>* getSteps();

    // Module's process
    void p_monitoring();

    SC_HAS_PROCESS(SystemMonitor);
    SystemMonitor(sc_module_name,RegisterFile*,DataMemory*,unsigned long long *cycleCount);

    ~SystemMonitor();
};

#endif // SYSTEMMONITOR_H
