/*
--------------------------------------------------------------------------------
PROJECT: MipsSimulator
MODULE : DataMemory
FILE   : DataMemory.cpp
--------------------------------------------------------------------------------
*/

#include "DataMemory.h"

DataMemory::DataMemory(sc_module_name mn) : sc_module(mn) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor Data Memory" << std::endl;
#endif

    SC_METHOD(p_write);
    dont_initialize();
    sensitive << i_CLK.pos();

    SC_METHOD(p_read);
    dont_initialize();
    sensitive <<i_ADDRESS << i_RD_ENABLE;

}

////////////////////////////////////////////////////////////////////////////////
void DataMemory::p_read() {
////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_METHODS
    std::cout << "DataMemory::p_read" << std::endl;
#endif

    if( i_RD_ENABLE.read() == 1 ) {
//        std::map<unsigned int, sc_uint<32> >::iterator it;
//        it = m_DATA.find(i_ADDRESS.read());
//        if(it != m_DATA.end()) {
//            o_DATA_OUT.write( it->second );
//        } else {
//            o_DATA_OUT.write( 0 );
//        }
        o_DATA_OUT.write( m_DATA.find(i_ADDRESS.read())->second );
    } else {
        o_DATA_OUT.write(0);
    }


}

////////////////////////////////////////////////////////////////////////////////
void DataMemory::p_write() {
////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_METHODS
    std::cout << "DataMemory::p_write" << std::endl;
#endif

    sc_uint<32> v_KEY;

    v_KEY = i_ADDRESS.read();

    if( i_WR_ENABLE.read() ) {
        if( keyExists( v_KEY ) ) { // STL map do not support overwrite!
            m_DATA.erase( v_KEY ); // So first erase previous value to next write new value.
        }

        m_DATA.insert( std::pair<unsigned int,sc_uint<32> >(v_KEY, i_DATA_IN.read()) );
    }

}

bool DataMemory::keyExists(unsigned int key) {
#ifdef DEBUG_METHODS
    std::cout << "DataMemory::keyExists" << std::endl;
#endif

    if( this->m_DATA.count(key) == 0 ) {
        return false;
    } else {
        return true;
    }
}

bool DataMemory::initialize(const char *filename) {
#ifdef DEBUG_METHODS
    std::cout << "DataMemory::initialize" << std::endl;
#endif

    FILE* file;

    if( (file = fopen(filename,"rt") ) == NULL ) {
        printf("Falha ao abrir arquivo para carregar a memoria de dados");
        return false;
    }

    int scanResult = 0;
    unsigned int address = DATAMEM_BOTTOM_ADDRESS;
    do {
        int hexValue;
        scanResult = fscanf(file,"%x",&hexValue);
        m_DATA.insert( std::pair<unsigned int, sc_int<32> >(address,hexValue) );
        address += 4; // +4 addresses
    } while( scanResult != EOF );

    fclose(file);

    m_DATA.erase(address-4); // Remove duplicate last value

    return true;

}

void DataMemory::debug() {
#ifdef DEBUG_METHODS
    std::cout << "DataMemory::debug" << std::endl;
#endif

    if( m_DATA.empty() ) {
        printf("\nData Memory is EMPTY");
        return;
    }

    std::map<unsigned int, sc_uint<32> >::iterator it;

    printf("\nData Memory Contents");
    for( it = m_DATA.begin(); it != m_DATA.end(); it++ ) {
        unsigned int address = (*it).first;
        sc_uint<32> data =  (*it).second;
        unsigned int d = data.to_uint();
        printf("\nAddress[%#.8x]: %u",address,d);
    }
}
