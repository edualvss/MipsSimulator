#include "CycleStatus.h"

#ifdef DEBUG_METHODS
    #include <iostream>
#endif

CycleStatus::CycleStatus() {
#ifdef DEBUG_METHODS
    std::cout << "Constructor CycleStatus" << std::endl;
#endif

}

CycleStatus::~CycleStatus() {
#ifdef DEBUG_METHODS
    std::cout << "Destructor CycleStatus" << std::endl;
#endif

    delete instruction;

}

void CycleStatus::insert(unsigned int key, unsigned int value) {
#ifdef DEBUG_METHODS
    std::cout << "CycleStatus::insert" << std::endl;
#endif

    if( keyExists( key ) ) { // STL map do not support overwrite!
        dataMemory.erase( key ); // So first erase previous value to next write new value.
    }

    dataMemory.insert( std::pair<unsigned int,unsigned int>(key, value) );
}

bool CycleStatus::keyExists(unsigned int key) {
#ifdef DEBUG_METHODS
    std::cout << "CycleStatus::keyExists" << std::endl;
#endif

    if( this->dataMemory.count(key) == 0 ) {
        return false;
    } else {
        return true;
    }

}
