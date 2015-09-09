#include "AppControl.h"

#include "View/MainWindow.h"
#include "SimulModels/MipsTop.h"
#include "SimulModels/testbench.h"

#include "Model/Instruction.h"
#include "SimulModels/Decoder.h"
#include "SimulModels/RegisterFile.h"
#include "SimulModels/InstructionMemory.h"
#include "SimulModels/DataMemory.h"

#ifdef DEBUG_METHODS
    #include <iostream>
#endif


AppControl::AppControl(QObject *parent) : QObject(parent) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor AppControl" << std::endl;
#endif

    this->simulator = NULL;
    this->tester = NULL;
    this->clockTime = 10;
    this->initialized = false;
    this->mainWindow = static_cast<MainWindow*>(parent);

    connect(mainWindow,SIGNAL(sendDataFile(QString)),this,SLOT(receiveDataFile(QString)));
    connect(mainWindow,SIGNAL(sendInstructionFile(QString)),this,SLOT(receiveInstructionFile(QString)));
}

void AppControl::startApp() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::startApp" << std::endl;
#endif

    this->mainWindow->show();

}

void AppControl::initSimulator() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::initSimulator" << std::endl;
#endif

    this->simulator = new Mips("Mips");
    sc_clock         w_CLK("CLK",clockTime,SC_NS);      // System clock with 10 ns period
    sc_signal<bool>  w_RST;

    tester = new Testbench("Tester");
    tester->i_CLK(w_CLK);
    tester->o_RST(w_RST);

    Mips* mips = simulator;
    mips->i_CLK(w_CLK);
    mips->i_RST(w_RST);

    this->initialized = true;
}

void AppControl::endSimulator() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::endSimulator" << std::endl;
#endif

    delete tester;
    delete simulator;
    tester = 0;
    simulator = 0;
    this->initialized = false;

}

void AppControl::receiveDataFile(QString filename) {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::receiveDataFile" << std::endl;
#endif

    this->dataFile = filename;
    if( ! initialized ) {
        this->initSimulator();
    }

    this->simulator->c_DataMemory->initialize(filename.toStdString().c_str());

    this->mainWindow->loadDataMemory( this->simulator->c_DataMemory );
}

void AppControl::receiveInstructionFile( QString filename ) {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::receiveInstructionFile" << std::endl;
#endif

    this->instructionFile = filename;

    if( initialized ) {
        this->endSimulator();
    }
    this->initSimulator();

    this->simulator->c_InstructionMemory->initialize( filename.toStdString().c_str() );

    this->mainWindow->loadInstructionMemory( this->simulator->c_InstructionMemory );
}
