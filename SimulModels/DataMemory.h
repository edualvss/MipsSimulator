/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : DataMemory
FILE   : DataMemory.h
--------------------------------------------------------------------------------
DESCRIPTION: Implements a data memory
--------------------------------------------------------------------------------
AUTHORS: Laboratory of Embedded and Distributed Systems (LEDS - UNIVALI)
--------------------------------------------------------------------------------
Last update on:
27/08/2015 - Eduardo Alves - Initial elaboration
--------------------------------------------------------------------------------
*/
#ifndef DATAMEMORY_H
#define DATAMEMORY_H

#include <systemc>
#include <map>

using namespace sc_core;
using namespace sc_dt;

// Stack/heap capacity is: ‭1,878,781,948 bytes
// In‬ words: ‭469,695,487 words of 4 bytes (32-bit)‬
#define STACK_TOP_ADDRESS  0x7FFFEFFC // First address of stack -> Last address of heap (dynamic memory)
#define STACK_DOWN_ADDRESS 0x10040000 // Last address of stack -> First address of heap (dynamic memory)

// Data memory capacity is: 196,608 bytes
// In words: 49,152 words of 4 bytes (32-bit)
#define DATAMEM_BOTTOM_ADDRESS 0x10010000   // First address of data memory
#define DATAMEM_UP_ADDRESS     0x1003FFFC   // Last address of data memory

#ifdef STATIC_DATA
#define STATICMEM_BOTTOM_ADDRESS 0x10000000 // Initial address of static data
#define STATICMEM_UP_ADDRESS     0x1000FFFC // Last address of static datas
#endif

#ifdef SINGLE_MEMORY
// Instruction memory capacity is: ‭264,241,152 bytes
// In words: ‬‭66,060,288 words of 4 bytes (32-bit) -> It is the number of instructions supported‬
#define INSTRUCTIONMEM_BOTTOM_ADDRESS 0x00400000 // Initial address from PC (Program Counter) - First address of instruction memory
#define INSTRUCTIONMEM_UP_ADDRESS     0x0FFFFFFC // Last address of instruction memory
#endif

// IMPORTANT: These values do not match the real "MIPS"
// but they were specified to keep compatibility with MARS (Mips Assembler and Runtime Simulator)

/* Memory organization is: (read bottom-up)
 *     |---------------| Real TOP                         => 0x7FFFFFFC
 *     |       .       |
 *     |     Stack     |$sp (dynamic memory top [stack] ) => 0x7FFFEFFC -- Compatibility with MARS
 *     |       .       |
 *     |       .       |
 *     |       .       |
 *     |  Stack/Heap   |
 *     |       .       |
 *     |       .       |
 *     |       .       |
 *     |     Heap      | MARS(dynamic memory down)         => 0x10040000 -- Compatibility with MARS
 *     |   ..DATA..    | MARS(last data memory address)    => 0x1003FFFC
 *     |       .       |
 *     |       .       |
 *     |   ..DATA..    | MARS(first data memory address)   => 0x10010000
 *     | ..Segmented.. |
 *     |---------------| $gp (real dynamic memory start)   => 0x10008000
 *     |    Static     |
 *     |       .       |
 *     |       .       |
 *     |    Static     |
 *     |---------------|                                   => 0x10000000
 *     |Text/Code(Ins) |
 *     | Instructions  |
 *     |       .       |
 *     |       .       |
 *     | Instructions  | PC(Program Counter) First Address => 0x00400000
 *     |---------------|
 *     |   Reserved    |
 *     |       .       |
 *     |---------------|                                   => 0x00000000
 *
 */

class DataMemory : public sc_module {
private:
    bool keyExists(unsigned int key);
public:

    // Interface
    sc_in<bool>           i_CLK;       // Clock
    sc_in<bool>           i_WR_ENABLE; // Write enable
    sc_in<bool>           i_RD_ENABLE; // Read enable
    sc_in<sc_uint<32> >   i_DATA_IN;   // Input data channel
    sc_in<sc_uint<32> >   i_ADDRESS;   // Address channel
    sc_out<sc_uint<32> >  o_DATA_OUT;  // Output data channel

    // Internal data structure for memory
    std::map<unsigned int, sc_uint<32> > m_DATA; // Data memory (like "RAM")

    // Module's process
    void p_write();
    void p_read();

    // Auxiliar member function
    bool initialize(const char* filename);
    void debug();

    SC_HAS_PROCESS(DataMemory);
    DataMemory(sc_module_name);
};

#endif // DATAMEMORY_H
