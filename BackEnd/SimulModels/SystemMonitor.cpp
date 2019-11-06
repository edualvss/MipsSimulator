#include "SystemMonitor.h"

#include "Model/CycleStatus.h"
#include "RegisterFile.h"
#include "Decoder.h"
#include "DataMemory.h"

#ifdef DEBUG_METHODS
    #include <iostream>
#endif

SystemMonitor::SystemMonitor(sc_module_name mn, RegisterFile* rf,
                             DataMemory *dm, unsigned long long *cycleCount)
    : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor SystemMonitor" << std::endl;
#endif


    SC_METHOD(p_monitoring);
    dont_initialize();
    sensitive << i_CLK.pos();

    this->steps = new std::vector<CycleStatus *>();
    this->regFile = rf;
    this->dataMemory = dm;
    this->cycleCount = cycleCount;
}

SystemMonitor::~SystemMonitor() {
#ifdef DEBUG_METHODS
    std::cout << "Destructor SystemMonitor" << std::endl;
#endif
    for(unsigned int i = 0; i < steps->size(); i++) {
        CycleStatus* cs = steps->at(i);
        delete cs;
    }
    steps->clear();
    delete steps;

}

//////////////////////////////////////////////////////////////////
void SystemMonitor::p_monitoring() {
//////////////////////////////////////////////////////////////////
#ifdef DEBUG_METHODS
    std::cout << "SystemMonitor::p_monitoring" << std::endl;
#endif

    CycleStatus* step = new CycleStatus();
    step->instruction     = Decoder::getInstructionDecoded( i_INSTRUCTION.read().to_uint() );
    copyDataMemory(&dataMemory->m_DATA,step);
    step->cycleNumber     = *cycleCount             ;
    step->currentPC       = i_CURRENT_PC.read()     ;
    step->nextPC          = i_NEXT_PC.read()        ;
    step->rf_readAddress1 = i_rf_RD_ADDRESS_0.read();
    step->rf_readAddress2 = i_rf_RD_ADDRESS_1.read();
    step->rf_writeAddress = i_rf_WR_ADDRESS.read()  ;
    step->rf_dataIn       = i_rf_DATA_IN.read()     ;
    step->rf_dataOut1     = i_rf_DATA_OUT_0.read()  ;
    step->rf_dataOut2     = i_rf_DATA_OUT_1.read()  ;
    step->alu_op          = i_ula_OP.read()         ;
    step->alu_in1         = i_ula_IN_0.read()       ;
    step->alu_in2         = i_ula_IN_1.read()       ;
    step->alu_zero        = i_ula_ZERO.read()       ;
    step->dataOut         = i_ula_OUT.read()        ;
    step->dvc             = i_DVC.read()            ;
    step->branch          = i_BRANCH.read()         ;
    step->jr              = i_JR.read()             ;
    step->jal             = i_JAL.read()            ;
    step->dvi             = i_DVI.read()            ;
    step->regDst          = i_REG_DST.read()        ;
    step->memRead         = i_MEM_RD.read()         ;
    step->memToReg        = i_MEM_TO_REG.read()     ;
    step->memWrite        = i_MEM_WR.read()         ;
    step->aluSrc          = i_ALU_SRC.read()        ;
    step->regWrite        = i_REG_WRITE.read()      ;

    for( int i = 0; i < 32; i++ ) {
        step->rf_registers[i] = regFile->m_REGISTERS[i];
    }

    steps->push_back(step);
}

void SystemMonitor::copyDataMemory(std::map<unsigned int, sc_uint<32> > *src,CycleStatus* step) {
#ifdef DEBUG_METHODS
    std::cout << "SystemMonitor::copyDataMemory" << std::endl;
#endif

    std::map<unsigned int, sc_uint<32> >::iterator it;

    for( it = src->begin(); it != src->end(); it++ ) {
        unsigned int addr = (*it).first;
        unsigned int val = it->second;
        step->insert(addr,val);
    }

}

std::vector<CycleStatus*>* SystemMonitor::getSteps() {
#ifdef DEBUG_METHODS
    std::cout << "SystemMonitor::getSteps" << std::endl;
#endif
    return this->steps;
}
