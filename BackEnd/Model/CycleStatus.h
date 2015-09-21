#ifndef CYCLESTATUS_H
#define CYCLESTATUS_H

#include "Instruction.h"
#include <map>

class CycleStatus {
private:
    bool keyExists(unsigned int key);
public:
    void insert(unsigned int key, unsigned int value);
    // General
    unsigned long long int cycleNumber;
    // Program Counter
    unsigned int currentPC;
    unsigned int nextPC;
    // Instruction memory
    Instruction* instruction;
    // Register file
    unsigned int   rf_registers[32];
    unsigned short rf_readAddress1;
    unsigned short rf_readAddress2;
    unsigned short rf_writeAddress;
    unsigned int   rf_dataIn;
    unsigned int   rf_dataOut1;
    unsigned int   rf_dataOut2;
    // ALU
    unsigned short alu_op;
    unsigned int   alu_in1;
    unsigned int   alu_in2;
    bool           alu_zero;
    // Data memory
    std::map<unsigned int, unsigned int> dataMemory;
    unsigned int dataOut;
    // Control
    bool dvc;
    bool branch;
    bool jr;
    bool jal;
    bool dvi;
    bool regDst;
    bool memRead;
    bool memToReg;
    bool memWrite;
    bool aluSrc;
    bool regWrite;

    explicit CycleStatus();
    ~CycleStatus();

};

#endif // CYCLESTATUS_H
