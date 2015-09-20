/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : Decoder
FILE   : Decoder.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements a instruction decoder
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
--------------------------------------------------------------------------------
Last update on:
27/08/2015 - Eduardo Alves - Initial elaboration
--------------------------------------------------------------------------------
*/
#ifndef DECODER_H
#define DECODER_H

class Instruction;

class Decoder {
    Decoder();
public:

    static Instruction* getInstructionDecoded(unsigned int instruction);
    static Instruction* getInstructionDecoded(Instruction* ins);


};

#endif // DECODER_H
