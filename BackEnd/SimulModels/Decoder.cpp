/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : Decoder
FILE   : Decoder.cpp
--------------------------------------------------------------------------------
*/
#include "Decoder.h"
#include "RegisterFile.h"

#include "Model/Instruction.h"

Decoder::Decoder(sc_module_name mn) : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor Decoder" << std::endl;
#endif

    SC_METHOD(p_decode);
    dont_initialize();
    sensitive << i_DATA_IN;

}

////////////////////////////////////////////////////////////////////////////////
void Decoder::p_decode() {
////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_METHODS
    std::cout << "Decoder::p_decode" << std::endl;
#endif

    // Temporary variable for manipulation bit vector
    sc_uint<32> v_DATA_IN;

    v_DATA_IN = i_DATA_IN.read();

    o_DATA_IMED26.write(v_DATA_IN(25,0));
    o_DATA_IMED16.write(v_DATA_IN(15,0));
    o_DATA_OP.write(v_DATA_IN(31,26));
    o_DATA_RS.write(v_DATA_IN(25,21));
    o_DATA_RT.write(v_DATA_IN(20,16));
    o_DATA_RD.write(v_DATA_IN(15,11));
    o_DATA_SHAMT.write( v_DATA_IN(10,6) );
    o_DATA_FUNCTION.write(v_DATA_IN(5,0));
}

Instruction* Decoder::getInstructionDecoded() {
#ifdef DEBUG_METHODS
    std::cout << "Decoder::getInstructionDecoded" << std::endl;
#endif

    return Decoder::getInstructionDecoded(i_DATA_IN.read());
}

Instruction* Decoder::getInstructionDecoded(unsigned int instruction) {
#ifdef DEBUG_METHODS
    std::cout << "Decoder::getInstructionDecoded" << std::endl;
#endif

    /* Instructions decoded
     * General
     * --------------------------------------------------------------
     *   OP      | Instruction | Format |  Addressing Mode  | Note
     * ----------|-------------|--------|-------------------|--------
     *  000000   | R-format    |   R    | Register  (see next table)
     * ----------|-------------|--------|-------------------|--------
     *  000010   | j           |   J    | Pseudodirect      | Jump unconditionally
     *  000011   | jal         |   J    | Pseudodirect      | Jump and link
     *  000100   | beq         |   I    | PC-relative       | Branch if equal
     *  000101   | bne         |   I    | PC-relative       | Branch if not equal
     *  001000   | addi        |   I    | Immediate         | Addition immediate with overflow
     *  001010   | slti        |   I    | Immediate         | Set less than immediate
     *  001100   | andi        |   I    | Immediate         | Bitwise AND immediate
     *  001101   | ori         |   I    | Immediate         | Bitwise OR immediate
     *  001110   | xori        |   I    | Immediate         | Bitwise XOR immediate
     *  001111   | lui         |   I    | Immediate         | Load upper immediate
     *  100011   | lw          |   I    | Base              | Store word
     *  101011   | sw          |   I    | Base              | Load word
     *
     *  R-format (OP: 000000) - Addressing: Register
     * ----------------------------------------------------
     *  Function | Instruction | Note
     *   000000  | sll         | Shift left logical
     *   000010  | srl         | Shift right logival
     *   001000  | jr          | Jump register
     *   001100  | syscall     | % Issue a system call (not implemented)
     *   001101  | break       | % Break execution Terminate program execution with exception (not implemented)
     *   100000  | add         | Addition with overflow
     *   100010  | sub         | Subtraction with overflow
     *   100100  | and         | Bitwise AND
     *   100101  | or          | Bitwise OR
     *   100110  | xor         | Bitwise XOR
     *   100111  | nor or (not)| Bitwise NOR (maybe bit inversion)
     *
     *  % Not implemented
     */
    sc_uint<32> v_INS;
    v_INS = instruction;

    unsigned int op = v_INS(31,26);
    unsigned int rs = v_INS(25,21);
    unsigned int rt = v_INS(20,16);
    unsigned int rd = v_INS(15,11);
    unsigned int shamt = v_INS(10,6);
    unsigned int function = v_INS(5,0);
    unsigned int imed26 = v_INS(25,0);
    unsigned int imed16 = v_INS(15,0);

    Instruction* ins = new Instruction(instruction);

    ins->setOp(op);
    ins->setRs(rs);
    ins->setRt(rt);
    ins->setRd(rd);
    ins->setShamt(shamt);
    ins->setFunction(function);
    ins->setImed26(imed26);
    ins->setImed16(imed16);
    ins->setRsName(RegisterFile::REG_NAMES[rs]);
    ins->setRtName(RegisterFile::REG_NAMES[rt]);
    ins->setRdName(RegisterFile::REG_NAMES[rd]);

    // To minimize code, by default
    // lets assuming that the Instruction is Format I and addressing mode is immediate
    ins->setFormat( Instruction::I );
    ins->setFormatName(Instruction::INSTRUCTION_FORMAT_NAME[Instruction::I]);
    ins->setAddressingMode(Instruction::Immediate);
    ins->setAddressingModeName(Instruction::ADDRESSING_MODE_NAME[Instruction::Immediate]);

    if( instruction == 0 ) {
        ins->setInstructionMnemonic("nop");
        return ins;
    }

    switch(op) { // Decode general instructions
        case 0b000000: { // R-format
            ins->setFormat( Instruction::R );
            ins->setFormatName(Instruction::INSTRUCTION_FORMAT_NAME[Instruction::R]);
            ins->setAddressingMode(Instruction::Register);
            ins->setAddressingModeName(Instruction::ADDRESSING_MODE_NAME[Instruction::Register]);
            switch(function) { // Decode R-format instructions
                case 0b000000: { // sll
                    ins->setInstructionMnemonic("sll");
                    break;
                }
                case 0b000010: { // srl
                    ins->setInstructionMnemonic("srl");
                    break;
                }
                case 0b001000: { // jr
                    ins->setInstructionMnemonic("jr");
                    break;
                }
                case 0b001100: { // syscall (decoded but not implemented)
                    ins->setInstructionMnemonic("syscall");
                    break;
                }
                case 0b001101: { // break (decoded but not implemented)
                    ins->setInstructionMnemonic("break");
                    break;
                }
                case 0b100000: { // add
                    ins->setInstructionMnemonic("add");
                    break;
                }
                case 0b100010: { // sub
                    ins->setInstructionMnemonic("sub");
                    break;
                }
                case 0b100100: { // and
                    ins->setInstructionMnemonic("and");
                    break;
                }
                case 0b100101: { // or
                    ins->setInstructionMnemonic("or");
                    break;
                }
                case 0b100110: { // xor
                    ins->setInstructionMnemonic("xor");
                    break;
                }
                case 0b100111: { // nor
                    ins->setInstructionMnemonic("nor");
                    break;
                }
                case 0b101010: {
                    ins->setInstructionMnemonic("slt");
                    break;
                }
                default: { // R-format instructions not implemented
                    ins->setInstructionMnemonic("RNI"); // R-format Not Implemented
                    break;
                }
            } // End function decode

            break;
        } // End R-format decode
        case 0b000010: { // j (jump)
            ins->setFormat( Instruction::J );
            ins->setFormatName(Instruction::INSTRUCTION_FORMAT_NAME[Instruction::J]);
            ins->setAddressingMode(Instruction::Pseudodirect);
            ins->setAddressingModeName(Instruction::ADDRESSING_MODE_NAME[Instruction::Pseudodirect]);
            ins->setInstructionMnemonic("j");
            break;
        }
        case 0b000011: { // jal
            ins->setFormat( Instruction::J );
            ins->setFormatName(Instruction::INSTRUCTION_FORMAT_NAME[Instruction::J]);
            ins->setAddressingMode(Instruction::Pseudodirect);
            ins->setAddressingModeName(Instruction::ADDRESSING_MODE_NAME[Instruction::Pseudodirect]);
            ins->setInstructionMnemonic("jal");
            break;
        }
        case 0b000100: { // beq
            ins->setAddressingMode(Instruction::PC_relative);
            ins->setAddressingModeName( Instruction::ADDRESSING_MODE_NAME[Instruction::PC_relative] );
            ins->setInstructionMnemonic("beq");
            break;
        }
        case 0b000101: { // bne
            ins->setAddressingMode(Instruction::PC_relative);
            ins->setAddressingModeName( Instruction::ADDRESSING_MODE_NAME[Instruction::PC_relative] );
            ins->setInstructionMnemonic("bne");
            break;
        }
        case 0b001000: { // addi
            ins->setInstructionMnemonic("addi");
            break;
        }
        case 0b001010: { // slti
            ins->setInstructionMnemonic("slti");
            break;
        }
        case 0b001100: { // andi
            ins->setInstructionMnemonic("andi");
            break;
        }
        case 0b001101: { // ori
            ins->setInstructionMnemonic("ori");
            break;
        }
        case 0b001110: { // xori
            ins->setInstructionMnemonic("xori");
            break;
        }
        case 0b001111: { // lui
            ins->setInstructionMnemonic("lui");
            break;
        }
        case 0b100011: { // lw
            ins->setAddressingMode(Instruction::Base);
            ins->setAddressingModeName(Instruction::ADDRESSING_MODE_NAME[Instruction::Base]);
            ins->setInstructionMnemonic("lw");
            break;
        }
        case 0b101011: { // sw
            ins->setAddressingMode(Instruction::Base);
            ins->setAddressingModeName(Instruction::ADDRESSING_MODE_NAME[Instruction::Base]);
            ins->setInstructionMnemonic("sw");
            break;
        }
        default: { // Instructions not implemented
            ins->setInstructionMnemonic("NI"); // Not Implemented
            break;
        }
    }
    return ins;

}

Instruction* Decoder::getInstructionDecoded(Instruction *ins) {
#ifdef DEBUG_METHODS
    std::cout << "Decoder::getInstructionDecoded" << std::endl;
#endif

    Instruction* i = Decoder::getInstructionDecoded(ins->getInstruction());
    delete ins;
    return i;
}
