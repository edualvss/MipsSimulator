#include "AppControl.h"

#include "View/MainWindow.h"
#include "SimulModels/MipsTop.h"
#include "SimulModels/testbench.h"

#include "Model/Instruction.h"
#include "SimulModels/Decoder.h"
#include "SimulModels/RegisterFile.h"
#include "SimulModels/InstructionMemory.h"
#include "SimulModels/DataMemory.h"
#include "SimulModels/SystemMonitor.h"
#include "Model/CycleStatus.h"

#include <time.h>

#ifdef DEBUG_METHODS
    #include <iostream>
#endif

#include <QDebug>

AppControl::AppControl(QObject *parent) : QObject(parent) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor AppControl" << std::endl;
#endif

    this->simulator = NULL;
    this->tester = NULL;
    this->steps = NULL;
    this->clockTime = 10;
    this->mainWindow = static_cast<MainWindow*>(parent);

    this->step = 0;
    this->started = false;
    this->simulated = false;
    this->instructionMemoryLoaded = false;
    this->dataMemoryLoaded = false;

    connect(mainWindow,SIGNAL(sendDataFile(QString)),this,SLOT(receiveDataFile(QString)));
    connect(mainWindow,SIGNAL(sendInstructionFile(QString)),this,SLOT(receiveInstructionFile(QString)));
    connect(mainWindow,SIGNAL(simulate()),this,SLOT(simulate()));
    connect(mainWindow,SIGNAL(nextStep()),this,SLOT(nextStep()));
    connect(mainWindow,SIGNAL(previousStep()),this,SLOT(previousStep()));
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

    sc_curr_simcontext = new sc_simcontext;
    sc_default_global_context = sc_curr_simcontext;

    this->simulator = new Mips("Mips");
    this->tester = new Testbench("MipsClockGeneretor");
    this->started = true;
    if( instructionMemoryLoaded ) {
        this->simulator->c_InstructionMemory->initialize( this->instructionFile.toStdString().c_str() );
    }
    if( dataMemoryLoaded ) {
        this->simulator->c_DataMemory->initialize( this->dataFile.toStdString().c_str() );
    }

}

void AppControl::simulate() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::simulate" << std::endl;
#endif

    if( simulated ) {
        endSimulator();
        initSimulator();
    }

    if( !instructionMemoryLoaded ) {
        this->mainWindow->showMessage( tr("Instruction memory not initialized") );
        return;
    }

    step = 0;

    sc_clock         w_CLK("CLK",10,SC_NS);      // System clock with 10 ns period
    sc_signal<bool>  w_RST;

    Testbench* tb = tester;
    tb->i_CLK(w_CLK);
    tb->o_RST(w_RST);

    Mips* m = simulator;
    m->i_CLK(w_CLK);
    m->i_RST(w_RST);

    clock_t t0 = clock();
    sc_start();
    clock_t t1 = clock();

    this->updateSteps( m->c_systemMonitor->getSteps() );

    delete m;
    delete tb;
    delete sc_curr_simcontext;
    sc_curr_simcontext = 0;

    simulationTime = (t1 - t0);
    simulated = true;
}

void AppControl::updateSteps(std::vector<CycleStatus *> *newSteps) {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::updateSteps" << std::endl;
#endif

    if( steps != NULL ) {
        for( unsigned int i = 0; i < steps->size(); i++ ) {
            CycleStatus* cs = steps->at(i);
            delete cs;
        }
        steps->clear();
        delete steps;
    }
    steps = newSteps;

}

void AppControl::endSimulator() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::endSimulator" << std::endl;
#endif

    started = false;
    simulated = false;

}

void AppControl::receiveDataFile(QString filename) {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::receiveDataFile" << std::endl;
#endif

    this->dataFile = filename;
    this->dataMemoryLoaded = true;
    if( !started ) {
        initSimulator();
    } else {
        if( simulated ) {
            endSimulator();
            initSimulator();
        }
        this->simulator->c_DataMemory->initialize( filename.toStdString().c_str() );
    }

    this->mainWindow->loadDataMemory( simulator->c_DataMemory );
}

void AppControl::receiveInstructionFile( QString filename ) {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::receiveInstructionFile" << std::endl;
#endif

    this->instructionFile = filename;
    this->instructionMemoryLoaded = true;
    if( !started ) {
        initSimulator();
    } else {
        if( simulated ) {
            endSimulator();
            initSimulator();
        }
        this->simulator->c_InstructionMemory->initialize( filename.toStdString().c_str() );
    }

    this->mainWindow->loadInstructionMemory( simulator->c_InstructionMemory );

}

void AppControl::nextStep() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::nextStep" << std::endl;
#endif

    if( !simulated ) {
        this->mainWindow->showMessage(tr("Not simulated"));
        return;
    }

    if( step >= steps->size()-1 ) {
        this->mainWindow->showMessage(tr("There is no next step"));
        return;
    }

    step++;
    this->mainWindow->updateStatus( steps->at(step) );
}

void AppControl::previousStep() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::previousStep" << std::endl;
#endif
    if(!simulated) {
        this->mainWindow->showMessage(tr("Not simulated"));
        return;
    }

    if( step <= 0 ) {
        this->mainWindow->showMessage(tr("There is no previous step"));
        return;
    }

    step--;
    this->mainWindow->updateStatus( steps->at(step) );

}
