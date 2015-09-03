/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : RegisterFile
FILE   : RegisterFile.cpp
--------------------------------------------------------------------------------
*/

#include "RegisterFile.h"

// Register names by position in array
const std::string RegisterFile::REG_NAMES[32] =    \
        {"$zero","$at","$v0","$v1","$a0",\
        "$a1","$a2","$a3","$t0","$t1",   \
        "$t2","$t3","$t4","$t5","$t6",   \
        "$t7","$s0","$s1","$s2","$s3",   \
        "$s4","$s5","$s6","$s7","$t8",   \
        "$t9","$k0","$k1","$gp","$sp",   \
        "$fp","$ra"};

RegisterFile::RegisterFile(sc_module_name mn) : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor RegisterFile" << std::endl;
#endif

    SC_METHOD(p_read);
    dont_initialize();
    sensitive << i_RD_ADDRESS_0 << i_RD_ADDRESS_1;

    SC_METHOD(p_write);
    dont_initialize();
    sensitive << i_CLK.pos();

    for( int i = 0; i < 32; i++ ) {
        m_REGISTERS[i] = 0;
    }
    m_REGISTERS[28] = 0x10008000; // $gp = global pointer
    m_REGISTERS[29] = 0x7fffeffc; // $sp = Stack pointer

}

////////////////////////////////////////////////////////////////////////////////
void RegisterFile::p_read() {
////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_METHODS
    std::cout << "RegisterFile::p_read" << std::endl;
#endif

    o_DATA_OUT_0.write( m_REGISTERS[i_RD_ADDRESS_0.read()] );
    o_DATA_OUT_1.write( m_REGISTERS[i_RD_ADDRESS_1.read()] );

}

////////////////////////////////////////////////////////////////////////////////
void RegisterFile::p_write() {
////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_METHODS
    std::cout << "RegisterFile::p_write" << std::endl;
#endif

    if( i_WR_ENABLE.read() ) {
        m_REGISTERS[i_WR_ADDRESS.read()] = i_DATA_IN.read();
    }

}
sc_uint<32>* RegisterFile::getRegistersValues() {
#ifdef DEBUG_METHODS
    std::cout << "RegisterFile::getRegistersValues" << std::endl;
#endif
    return m_REGISTERS;
}

sc_uint<32> RegisterFile::getRegisterValue(int pos) {
#ifdef DEBUG_METHODS
    std::cout << "RegisterFile::getRegisterValue" << std::endl;
#endif

    if( pos < 0 || pos > 31 ) {
        return 0;
    }
    return m_REGISTERS[pos];
}

sc_uint<32> RegisterFile::getRegisterValue(std::string regName) {
#ifdef DEBUG_METHODS
    std::cout << "RegisterFile::getRegisterValue" << std::endl;
#endif

    int pos = getPosition(regName);
    if(pos == -1) {
        return 0;
    }

    return m_REGISTERS[pos];
}


int RegisterFile::getPosition(std::string regName) {
#ifdef DEBUG_METHODS
    std::cout << "RegisterFile::getPosition" << std::endl;
#endif

    for( int i = 0; i < 32; i++ ) {
        if( regName.compare(REG_NAMES[i]) == 0) {
            return i;
        }
    }

    return -1;
}

void RegisterFile::debug() {
#ifdef DEBUG_METHODS
    std::cout << "RegisterFile::debug" << std::endl;
#endif

    for( int i = 0; i < 32; i++ ) {
        std::cout << "\nREG: " << REG_NAMES[i] << ": " << std::hex << m_REGISTERS[i] << std::dec;
    }

}
