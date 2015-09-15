#include "AppControl.h"

#include "View/MainWindow.h"
#include "SimulModels/MipsTop.h"
#include "SimulModels/testbench.h"

#include "Model/Instruction.h"
#include "SimulModels/Decoder.h"
#include "SimulModels/RegisterFile.h"
#include "SimulModels/InstructionMemory.h"
#include "SimulModels/DataMemory.h"

#include <time.h>

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
    this->mainWindow = static_cast<MainWindow*>(parent);

    connect(mainWindow,SIGNAL(sendDataFile(QString)),this,SLOT(receiveDataFile(QString)));
    connect(mainWindow,SIGNAL(sendInstructionFile(QString)),this,SLOT(receiveInstructionFile(QString)));
    connect(mainWindow,SIGNAL(simulate()),this,SLOT(simulate()));
}

void AppControl::startApp() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::startApp" << std::endl;
#endif

    this->mainWindow->show();

}

int sc_main(int , char**) {
    return 0;
}

void AppControl::initSimulator() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::initSimulator" << std::endl;
#endif

    sc_core::sc_elab_and_sim(0,0);

    this->simulator = new Mips("Mips");
    this->simulator->c_DataMemory->initialize(dataFile.toStdString().c_str());
    this->simulator->c_InstructionMemory->initialize(instructionFile.toStdString().c_str());
    sc_clock         w_CLK("CLK",clockTime,SC_NS);      // System clock with 10 ns period
    sc_signal<bool>  w_RST;

    tester = new Testbench("Tester");
    tester->i_CLK(w_CLK);
    tester->o_RST(w_RST);

    Mips* mips = simulator;
    mips->i_CLK(w_CLK);
    mips->i_RST(w_RST);

    clock_t t0 = clock();
    sc_start();
//    endSimulator();
    clock_t t1 = clock();

    std::cout << "Tempo de simulacao: " << (t1-t0) << std::endl;
}

void AppControl::endSimulator() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::endSimulator" << std::endl;
#endif

    delete tester;
    delete simulator;
    tester = 0;
    simulator = 0;

}

void AppControl::receiveDataFile(QString filename) {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::receiveDataFile" << std::endl;
#endif

    this->dataFile = filename;

}

void AppControl::receiveInstructionFile( QString filename ) {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::receiveInstructionFile" << std::endl;
#endif

    this->instructionFile = filename;

}

void AppControl::simulate() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::simulate" << std::endl;
#endif
    initSimulator();

}
