/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : Instruction
FILE   : Instruction.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements a instruction data type with auxiliar operations
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
--------------------------------------------------------------------------------
Last update on:
31/08/2015 - Eduardo Alves - Initial elaboration
--------------------------------------------------------------------------------
*/

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

class Instruction {
public:
    enum InstructionFormat { R=0,  // Arithmetic instruction format
                             I,    // Transfer, branch, immediate format
                             J };  // Jump instruction format
    static const char* INSTRUCTION_FORMAT_NAME[3];

    enum InstructionFields {OP=0,      // Ins[31..26] - 6-bit
                            RS,        // Ins[25..21] - 5-bit
                            RT,        // Ins[20..16] - 5-bit
                            RD,        // Ins[15..11] - 5-bit
                            SHAMT,     // Ins[10..6]  - 5-bit
                            FUNCTION,  // Ins[5..0]   - 6-bit
                            IMED26,    // Ins[25..0]  - 26-bit
                            IMED16 };  // Ins[15..0]  - 16-bit

    enum AddressingMode {Register=0,      // Via register - Where the operand is a register
                         Base,            // Base addressing - Where the operand is at the memory
                         Immediate,       // Immediate - Where the operand is a constant within the instruction itself
                         PC_relative,     // PC-relative - Where the address is the sum of the PC and a constant in the instruction
                         Pseudodirect };  // Pseudodirect - Where the jump address is the 26-bit of the instruction concatenated with upper bits of the PC
    static const char* ADDRESSING_MODE_NAME[5];

    Instruction(unsigned int instruction);

    // Gets
    inline InstructionFormat getFormat             () { return format            ; }
    inline const char*       getFormatName         () { return formatName        ; }
    inline AddressingMode    getAddressingMode     () { return addressingMode    ; }
    inline const char*       getAddressingModeName () { return addressingModeName; }
    inline const char*       getInstructionMnemonic() { return insMnemonic       ; }
    inline unsigned int      getOp                 () { return _op               ; }
    inline const char*       getRsName             () { return rsRegName         ; }
    inline unsigned int      getRs                 () { return _rs               ; }
    inline const char*       getRtName             () { return rtRegName         ; }
    inline unsigned int      getRt                 () { return _rt               ; }
    inline const char*       getRdName             () { return rdRegName         ; }
    inline unsigned int      getRd                 () { return _rd               ; }
    inline unsigned int      getShamt              () { return _shamt            ; }
    inline unsigned int      getFunction           () { return _function         ; }
    inline unsigned int      getImed16             () { return _imed16           ; }
    inline unsigned int      getImed26             () { return _imed26           ; }
    inline unsigned int      getInstruction        () { return instruction       ; }

    // Sets
    inline void setFormat             (InstructionFormat insForm   ) { format             = insForm  ; }
    inline void setFormatName         (const char*       name      ) { formatName         = name     ; }
    inline void setAddressingMode     (AddressingMode    mode      ) { addressingMode     = mode     ; }
    inline void setAddressingModeName (const char*       name      ) { addressingModeName = name     ; }
    inline void setInstructionMnemonic(const char*       mnemonic  ) { insMnemonic        = mnemonic ; }
    inline void setOp                 (unsigned int      op        ) { _op                = op       ; }
    inline void setRsName             (const char*       rsName    ) { rsRegName          = rsName   ; }
    inline void setRs                 (unsigned int      rs        ) {  _rs               = rs       ; }
    inline void setRtName             (const char*       rtName    ) { rtRegName          = rtName   ; }
    inline void setRt                 (unsigned int      rt        ) {  _rt               = rt       ; }
    inline void setRdName             (const char*       rdName    ) { rdRegName          = rdName   ; }
    inline void setRd                 (unsigned int      rd        ) {  _rd               = rd       ; }
    inline void setShamt              (unsigned int      shamt     ) {  _shamt            = shamt    ; }
    inline void setFunction           (unsigned int      function  ) {  _function         = function ; }
    inline void setImed16             (unsigned int      imed16    ) {  _imed16           = imed16   ; }
    inline void setImed26             (unsigned int      imed26    ) {  _imed26           = imed26   ; }
    inline void setInstruction        (unsigned int      ins       ) { instruction        = ins      ; }

    char *getFormattedInstruction();

    void debug();

private:

    unsigned int instruction;

    InstructionFormat format;
    const char* formatName;

    AddressingMode addressingMode;
    const char* addressingModeName;

    const char* insMnemonic;
    unsigned int  _op;
    const char* rsRegName;
    unsigned int  _rs;
    const char* rtRegName;
    unsigned int  _rt;
    const char* rdRegName;
    unsigned int  _rd;
    unsigned int  _shamt;
    unsigned int  _function;
    unsigned int  _imed16;
    unsigned int  _imed26;
};

#endif // INSTRUCTION_H
