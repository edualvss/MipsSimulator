#include <systemc>
#include "SimulModels/testbench.h"
#include "SimulModels/MipsTop.h"
#include "SimulModels/InstructionMemory.h"
#include "SimulModels/DataMemory.h"
#include "SimulModels/SystemMonitor.h"
#include "Model/CycleStatus.h"

#include <sys/time.h>

using namespace sc_core;

#include <iostream>

#ifdef DEBUG_METHODS
    #include <iostream>
#endif

enum FileFormat{JSON=0,CSV};

// Calculte the difference between two times
void timeval_diff(struct timeval *difference,
             struct timeval *end_time,
             struct timeval *start_time
            ) {

    struct timeval temp_diff;

    if(difference==NULL) {
        difference=&temp_diff;
    }

    difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
    difference->tv_usec=end_time->tv_usec-start_time->tv_usec;

    /* Using while instead of if below makes the code slightly more robust. */

    while(difference->tv_usec<0) {
        difference->tv_usec+=1000000;
        difference->tv_sec -=1;
    }
}

// Write the output file. NOTE: file already open, also it do not need close
void writeStepsInJson(FILE* file, std::vector<CycleStatus *>* steps,
                long unsigned int simTimeSec, long unsigned int simTimeUSec) {
    // Write in JSON format

    std::map<unsigned int,unsigned int>::iterator it;

    fprintf(file,"{");
    fprintf(file,"\n\t\"simTimeS\": %lu,",simTimeSec);
    fprintf(file,"\n\t\"simTimeUS\": %lu,",simTimeUSec);
    fprintf(file,"\n\n\t\"steps\": [");
    for( unsigned int i = 0; i < steps->size(); i++ ) {
        CycleStatus* cycle = steps->at(i);
        fprintf(file,"\n\t\t{");

        fprintf(file,"\n\t\t\t\"cycleNumber\": %llu,",cycle->cycleNumber);
        fprintf(file,"\n\t\t\t\"currentPC\": %u,",cycle->currentPC);
        fprintf(file,"\n\t\t\t\"nextPC\": %u,",cycle->nextPC);
        fprintf(file,"\n\t\t\t\"instruction\": %u,",cycle->instruction->getInstruction());
        fprintf(file,"\n\t\t\t\"rf_readAddress1\": %hu,",cycle->rf_readAddress1);
        fprintf(file,"\n\t\t\t\"rf_readAddress2\": %hu,",cycle->rf_readAddress2);
        fprintf(file,"\n\t\t\t\"rf_writeAddress\": %hu,",cycle->rf_writeAddress);
        fprintf(file,"\n\t\t\t\"rf_dataIn\": %u,",cycle->rf_dataIn);
        fprintf(file,"\n\t\t\t\"rf_dataOut1\": %u,",cycle->rf_dataOut1);
        fprintf(file,"\n\t\t\t\"rf_dataOut2\": %u,",cycle->rf_dataOut2);
        fprintf(file,"\n\t\t\t\"aluOp\": %hu,",cycle->alu_op);
        fprintf(file,"\n\t\t\t\"aluIn1\": %u,",cycle->alu_in1);
        fprintf(file,"\n\t\t\t\"aluIn2\": %u,",cycle->alu_in2);
        fprintf(file,"\n\t\t\t\"aluZero\": %s,", (cycle->alu_zero?"true":"false") );
        fprintf(file,"\n\t\t\t\"memDataOut\": %u,",cycle->dataOut);
        fprintf(file,"\n\t\t\t\"dvc\": %s,",(cycle->dvc?"true":"false"));
        fprintf(file,"\n\t\t\t\"branch\": %s,",(cycle->branch?"true":"false"));
        fprintf(file,"\n\t\t\t\"jr\": %s,",(cycle->jr?"true":"false"));
        fprintf(file,"\n\t\t\t\"jal\": %s,",(cycle->jal?"true":"false"));
        fprintf(file,"\n\t\t\t\"dvi\": %s,",(cycle->dvi?"true":"false"));
        fprintf(file,"\n\t\t\t\"regDst\": %s,",(cycle->regDst?"true":"false"));
        fprintf(file,"\n\t\t\t\"memRead\": %s,",(cycle->memRead?"true":"false"));
        fprintf(file,"\n\t\t\t\"memToReg\": %s,",(cycle->memToReg?"true":"false"));
        fprintf(file,"\n\t\t\t\"memWrite\": %s,",(cycle->memWrite?"true":"false"));
        fprintf(file,"\n\t\t\t\"aluSrc\": %s,",(cycle->aluSrc?"true":"false"));
        fprintf(file,"\n\t\t\t\"regWrite\": %s,",(cycle->regWrite?"true":"false"));
        fprintf(file,"\n\t\t\t\"RegFile\": [");
        for(unsigned int j = 0; j < 32; j++) {
            fprintf(file,"\n\t\t\t\t{\"address\": %u,\"value\": %u}",j,cycle->rf_registers[j]);
            if( j < 31 ) {
                fprintf(file,",");
            }
        }
        fprintf(file,"\n\t\t\t],");

        fprintf(file,"\n\t\t\t\"DataMem\": [");
        unsigned int x = 0;
        unsigned int memSize;
        memSize = cycle->dataMemory.size();
        for(it = cycle->dataMemory.begin(); it != cycle->dataMemory.end(); it++, x++) {
            fprintf(file,"\n\t\t\t\t{\"address\": %u, \"value\": %u}",it->first,it->second);
            if( x < memSize-1 ) {
                fprintf(file,",");
            }
        }
        fprintf(file,"\n\t\t\t]");

        fprintf(file,"\n\t\t}");
        if( i < (steps->size() - 1) ) {
            fprintf(file,",");
        }

    }

    fprintf(file,"\n\t]");
    fprintf(file,"\n}");

}

void writeStepsInCSV(FILE* file, std::vector<CycleStatus *>* steps,
                     long unsigned int simTimeSec,long unsigned int simTimeUSec) {

    fprintf(file,"SimTimeSec,SimTimeUSec");
    fprintf(file,"\n%lu,%lu",simTimeSec,simTimeUSec);

    fprintf(file,"\nCycleNumber,currentPC,nextPC,instruction,rf_readAddress1,rf_readAddress2,rf_writeAddress,rf_dataIn,rf_dataOut1,rf_dataOut2,aluOp,aluIn1,aluIn2,aluZero,memDataOut,dvc,branch,jr,jal,dvi,regDst,memRead,memToReg,memWrite,aluSrc,regWrite,reg0,reg1,reg2,reg3,reg4,reg5,reg6,reg7,reg8,reg9,reg10,reg11,reg12,reg13,reg14,reg15,reg16,reg17,reg18,reg19,reg20,reg21,reg22,reg23,reg24,reg25,reg26,reg27,reg28,reg29,reg30,reg31");
    fprintf(file,"\naddr1|mem1,addr2|mem2,addr3|mem3,...");
    fprintf(file,"\n#");

    // To iterate in data memory
    std::map<unsigned int,unsigned int>::iterator it;

    for( unsigned int i = 0; i < steps->size(); i++ ) {
        CycleStatus* cycle = steps->at(i);
        fprintf(file,"\n%llu,",cycle->cycleNumber);
        fprintf(file,"%u,",cycle->currentPC);
        fprintf(file,"%u,",cycle->nextPC);
        fprintf(file,"%u,",cycle->instruction->getInstruction());
        fprintf(file,"%hu,",cycle->rf_readAddress1);
        fprintf(file,"%hu,",cycle->rf_readAddress2);
        fprintf(file,"%hu,",cycle->rf_writeAddress);
        fprintf(file,"%u,",cycle->rf_dataIn);
        fprintf(file,"%u,",cycle->rf_dataOut1);
        fprintf(file,"%u,",cycle->rf_dataOut2);
        fprintf(file,"%hu,",cycle->alu_op);
        fprintf(file,"%u,",cycle->alu_in1);
        fprintf(file,"%u,",cycle->alu_in2);
        fprintf(file,"%u,",cycle->alu_zero);
        fprintf(file,"%u,",cycle->dataOut);
        fprintf(file,"%u,",cycle->dvc);
        fprintf(file,"%u,",cycle->branch);
        fprintf(file,"%u,",cycle->jr);
        fprintf(file,"%u,",cycle->jal);
        fprintf(file,"%u,",cycle->dvi);
        fprintf(file,"%u,",cycle->regDst);
        fprintf(file,"%u,",cycle->memRead);
        fprintf(file,"%u,",cycle->memToReg);
        fprintf(file,"%u,",cycle->memWrite);
        fprintf(file,"%u,",cycle->aluSrc);
        fprintf(file,"%u",cycle->regWrite);

        for(unsigned int j = 0; j < 32; j++) {
            fprintf(file,",%u",cycle->rf_registers[j]);
        }
        fprintf(file,"\n");

        unsigned int x = 0;
        unsigned int memSize;
        memSize = cycle->dataMemory.size();
        for(it = cycle->dataMemory.begin(); it != cycle->dataMemory.end(); it++, x++) {
            fprintf(file,"%u|%u",it->first,it->second);
            if( x < memSize-1 ) {
                fprintf(file,",");
            }
        }
    }

}

void writeFile(FileFormat format, FILE* file, std::vector<CycleStatus *>* steps,
               long unsigned int simTimeSec, long unsigned int simTimeUSec) {

    switch (format) {
        case JSON:
            writeStepsInJson(file,steps,simTimeSec,simTimeUSec);
            break;
        case CSV:
            writeStepsInCSV(file,steps,simTimeSec,simTimeUSec);
            break;
    }

}

int sc_main(int argc, char* argv[]) {
#ifdef DEBUG_METHODS
    std::cout << "sc_main" << std::endl;
#endif
    /// argv[0] -> Executable location
    /// argv[1] -> Temporary folder to write files
    /// argv[2] -> Instruction memory file
    /// argv[3] -> Data memory file

    FileFormat format = CSV;

    std::cout << "Args!" << std::endl;
    for( int i = 0; i < argc; i++ ) {
        std::cout << "\nArg[" << i << "]: " << argv[i];
    }

    if( argc < 3) {
        std::cout << "\nMissing arguments." \
                     "\nVerify: \n" \
                     "\n 1-) Destination folder to write files of simulation" \
                     "\n 2-) Instruction memory file" \
                     "\n 3-) Data memory file (optional)";
        return 1;
    }

    sc_clock         w_CLK("CLK",10,SC_NS);      // System clock with 10 ns period
    sc_signal<bool>  w_RST;

    Testbench* tb = new Testbench("ClockGenerator");
    tb->i_CLK(w_CLK);
    tb->o_RST(w_RST);

    Mips* m = new Mips("Mips",argv[1]);
    m->c_InstructionMemory->initialize(argv[2]);
    if( argc > 3 ) {
        m->c_DataMemory->initialize(argv[3]);
    }
    m->i_CLK(w_CLK);
    m->i_RST(w_RST);

    struct timeval startTime,endTime,timeDiff;


    gettimeofday(&startTime,0x0);
    sc_start();
    gettimeofday(&endTime,0x0);
    timeval_diff(&timeDiff,&endTime,&startTime);

    std::cout << "\nSimulation time in us: " << timeDiff.tv_usec;
    std::cout << "\nSimulation time in s: " << timeDiff.tv_sec;

    std::vector<CycleStatus *>* steps = m->c_systemMonitor->getSteps();

    // Writing output file!
    FILE* file;

    char tmpBuff[512];
    switch(format) {
        case JSON:
            sprintf(tmpBuff,"%s/SimulationSteps.json",argv[1]);
            break;
        case CSV:
            sprintf(tmpBuff,"%s/SimulationSteps.csv",argv[1]);
            break;
    }

    std::cout << "\nOutput file is:" << tmpBuff;

    if( (file = fopen(tmpBuff,"w")) == NULL) {
        std::cout << "\nIt is not possible open output file to write steps of simulation!";
        delete m;
        delete tb;
        return 2;
    }

    writeFile(format,file,steps,timeDiff.tv_sec,timeDiff.tv_usec);

    fclose(file);

    delete m;
    delete tb;

    std::cout << std::endl;
    return 0;
}
