#include "AppControl.h"

#include "View/MainWindow.h"

#include "Model/Instruction.h"
#include "Model/CycleStatus.h"
#include "Model/Decoder.h"
#include "Model/RegisterFile.h"

#include <QProcess>
#include <QDir>
#include <QApplication>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#ifdef DEBUG_METHODS
    #include <iostream>
#endif

AppControl::AppControl(QObject *parent) : QObject(parent) {
#ifdef DEBUG_METHODS
    std::cout << "Constructor AppControl" << std::endl;
#endif

    this->steps = NULL;
    this->mainWindow = static_cast<MainWindow*>(parent);

    this->step = 0;
    this->simulated = false;
    this->instructionMemoryLoaded = false;
    this->dataMemoryLoaded = false;

    this->simulationTimeS = 0;
    this->simulationTimeUS = 0;

    connect(mainWindow,SIGNAL(sendDataFile(QString)),this,SLOT(receiveDataFile(QString)));
    connect(mainWindow,SIGNAL(sendInstructionFile(QString)),this,SLOT(receiveInstructionFile(QString)));
    connect(mainWindow,SIGNAL(simulate()),this,SLOT(simulate()));
    connect(mainWindow,SIGNAL(nextStep()),this,SLOT(nextStep()));
    connect(mainWindow,SIGNAL(previousStep()),this,SLOT(previousStep()));
    connect(mainWindow,SIGNAL(viewSimulationTime()),this,SLOT(showSimulationTime()));
}

void AppControl::startApp() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::startApp" << std::endl;
#endif

    this->mainWindow->show();

}

void AppControl::simulate() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::simulate" << std::endl;
#endif

    if( !instructionMemoryLoaded ) {
        this->mainWindow->showMessage( tr("Instruction memory not initialized") );
        return;
    }

    // Disable simulate button
    this->mainWindow->setEnabledSimulate(false);

    // Create a process
    mipsProcess = new QProcess(this);
    connect(mipsProcess,SIGNAL(finished(int)),this,SLOT(endSimulation()));
    QString workDir = QDir::homePath()+"/MipsSimulator";
    mipsProcess->setWorkingDirectory(workDir);
    QDir d(workDir);
    if( !d.exists() ) {
        d.mkpath(".");
    }
    mipsProcess->setProgram( qApp->applicationDirPath()+"/Simulator/SysMips" );
    QStringList args;
    args.append( workDir );
    args.append( instructionFile );
    if( dataMemoryLoaded ) {
        args.append(dataFile);
    }
    mipsProcess->setArguments( args );
    QApplication::setOverrideCursor(Qt::WaitCursor);
    mipsProcess->start();
    this->step = 0;
}

void AppControl::endSimulation() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::endSimulation" << std::endl;
#endif

    this->loadStepsInCSV();


    this->mainWindow->resetRegisterFileTable();
    QApplication::restoreOverrideCursor();

    QMap<unsigned int,unsigned int>* counter = this->countInstructionAccess();
    this->mainWindow->updateInstructionCounter(counter);
    counter->clear();
    delete counter;

    mipsProcess->deleteLater();
    this->mainWindow->showMessage( tr("Simulated in: %1 s and %2 us")
                                   .arg(QString::number(simulationTimeS))
                                   .arg(QString::number(simulationTimeUS)));

    this->mainWindow->showMessageInStatusBar(tr("Simulated in: %1 s and %2 us")
                                             .arg(QString::number(simulationTimeS))
                                             .arg(QString::number(simulationTimeUS)),0);
    simulated = true;
    this->mainWindow->setEnabledSimulationTime(simulated);
    this->mainWindow->setEnabledSimulate(true);

}

QMap<unsigned int, unsigned int>* AppControl::countInstructionAccess() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::countInstructionAccess" << std::endl;
#endif

    // Key: PC | Value: Frequency
    QMap<unsigned int, unsigned int>* counter = new QMap<unsigned int,unsigned int>();
    for(unsigned int i = 1; i < steps->size(); i++) {
        CycleStatus* cycle = steps->at(i);
        unsigned int pcValue = cycle->currentPC;
        if( counter->contains(pcValue) ) {
            counter->insert(pcValue,counter->value(pcValue) + 1);
        } else {
            counter->insert(pcValue,1);
        }
    }

    return counter;

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


void AppControl::receiveDataFile(QString filename) {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::receiveDataFile" << std::endl;
#endif

    this->dataFile = filename;
    this->dataMemoryLoaded = true;

    std::vector<unsigned int>* data = this->loadFile(filename);

    if( data == NULL ) {
        this->mainWindow->showMessage(tr("Failed to open data memory file"));
        this->dataMemoryLoaded = false;
        return;
    }

    this->mainWindow->loadDataMemory( data );

    data->clear();
    delete data;
}

void AppControl::receiveInstructionFile( QString filename ) {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::receiveInstructionFile" << std::endl;
#endif

    this->instructionFile = filename;
    this->instructionMemoryLoaded = true;

    std::vector<unsigned int>* data = this->loadFile(filename);
    if( data == NULL ) {
        this->mainWindow->showMessage(tr("Failed to open instruction memory file"));
        this->instructionMemoryLoaded = false;
        return;
    }

    this->mainWindow->loadInstructionMemory( data );

    data->clear();
    delete data;
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

void AppControl::showSimulationTime() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::showSimulationTime" << std::endl;
#endif

    this->mainWindow->showMessage(tr("Simulated in: %1 s and %2 us")
                                  .arg(QString::number(simulationTimeS))
                                  .arg(QString::number(simulationTimeUS)));

}

std::vector<unsigned int>* AppControl::loadFile(QString filename) {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::loadFile" << std::endl;
#endif

    FILE* file;

    if( (file = fopen(filename.toStdString().c_str(),"rt") ) == NULL ) {
        return NULL;
    }

    int scanResult = 0;
    std::vector<unsigned int>* data = new std::vector<unsigned int>();
    do {
        int hexValue;
        scanResult = fscanf(file,"%x",&hexValue);
        data->push_back(hexValue);
    } while( scanResult != EOF );

    fclose(file);

    // Remove duplicate last value
    data->pop_back();

    return data;
}

void AppControl::loadStepsInCSV() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::loadStepsInCSV" << std::endl;
#endif

    QFile loadFile(QDir::homePath()+"/MipsSimulator/SimulationSteps.csv");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        this->mainWindow->showMessage(tr("Couldn't open simulations status file."));
        return;
    }

    std::vector<CycleStatus *>* newSteps = new std::vector<CycleStatus *>();

    QByteArray saveData = loadFile.readAll();
    loadFile.close();

    QList<QByteArray> lines = saveData.split('\n');
    QByteArray line1 = lines.at(1);
    line1 = line1.simplified();
    QList<QByteArray> times = line1.split(',');
    simulationTimeS = times[0].toULong();
    simulationTimeUS = times[1].toULong();

    for( int i = 5; i < lines.size(); i+=2 ) {
        CycleStatus* cycle = new CycleStatus();
        // General status
        QByteArray lineI = lines.at(i);
        lineI = lineI.simplified();
        QList<QByteArray> fieldsLineI = lineI.split(',');
        cycle->cycleNumber = fieldsLineI[0].toULongLong();
        cycle->currentPC = fieldsLineI[1].toUInt();
        cycle->nextPC = fieldsLineI[2].toUInt();
        cycle->instruction = Decoder::getInstructionDecoded( fieldsLineI[3].toUInt() );
        cycle->rf_readAddress1 = fieldsLineI[4].toUShort();
        cycle->rf_readAddress2 = fieldsLineI[5].toUShort();
        cycle->rf_writeAddress = fieldsLineI[6].toUShort();
        cycle->rf_dataIn = fieldsLineI[7].toUInt();
        cycle->rf_dataOut1 = fieldsLineI[8].toUInt();
        cycle->rf_dataOut2 = fieldsLineI[9].toUInt();
        cycle->alu_op = fieldsLineI[10].toUShort();
        cycle->alu_in1 = fieldsLineI[11].toUInt();
        cycle->alu_in2 = fieldsLineI[12].toUInt();
        cycle->alu_zero = fieldsLineI[13].toUInt();
        cycle->dataOut = fieldsLineI[14].toUInt();
        cycle->dvc = fieldsLineI[15].toUInt();
        cycle->branch = fieldsLineI[16].toUInt();
        cycle->jr = fieldsLineI[17].toUInt();
        cycle->jal = fieldsLineI[18].toUInt();
        cycle->dvi = fieldsLineI[19].toUInt();
        cycle->regDst = fieldsLineI[20].toUInt();
        cycle->memRead = fieldsLineI[21].toUInt();
        cycle->memToReg = fieldsLineI[22].toUInt();
        cycle->memWrite = fieldsLineI[23].toUInt();
        cycle->aluSrc = fieldsLineI[24].toUInt();
        cycle->regWrite = fieldsLineI[25].toUInt();

        // Register file
        for( int x = 0; x < 32; x++) {
            cycle->rf_registers[x] = fieldsLineI[26+x].toUInt();
        }

        // Data Memory
        QByteArray lineI2 = lines.at(i+1);
        lineI2 = lineI2.simplified();
        QList<QByteArray> fieldsLineI2 = lineI2.split(',');
        for( int x = 0; x < fieldsLineI2.size(); x++ ) {
            QList<QByteArray> memPos = fieldsLineI2.at(x).split('|');
            if( memPos.isEmpty() || memPos.size() < 2 ) {
                continue;
            }
            unsigned int address = memPos.at(0).toUInt(); // Address
            unsigned int value = memPos.at(1).toUInt(); // Value
            cycle->insert( address, value );
        }

        newSteps->push_back(cycle);
    }

    this->updateSteps(newSteps);

}

void AppControl::loadStepsInJson() {
#ifdef DEBUG_METHODS
    std::cout << "AppControl::loadStepsInJson" << std::endl;
#endif

    QFile loadFile(QDir::homePath()+"/MipsSimulator/SimulationSteps.json");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        this->mainWindow->showMessage(tr("Couldn't open simulations status file."));
        return;
    }

    std::vector<CycleStatus *>* newSteps = new std::vector<CycleStatus *>();

    QByteArray saveData = loadFile.readAll();
    loadFile.close();
    saveData = saveData.simplified();

    QJsonDocument loadDoc(  QJsonDocument::fromJson(saveData) );

    QJsonObject json = loadDoc.object();

    simulationTimeS = json["simTimeS"].toInt();
    simulationTimeUS = json["simTimeUS"].toInt();

    QJsonArray jsonSteps = json["steps"].toArray();
    long int i;
    for( i = 0; i < jsonSteps.size(); i++ ) {
        QJsonObject jsonStep = jsonSteps.at(i).toObject();
        CycleStatus* cycle = new CycleStatus();
        cycle->cycleNumber = jsonStep["cycleNumber"].toVariant().toULongLong();
        cycle->currentPC = jsonStep["currentPC"].toVariant().toUInt();
        cycle->nextPC = jsonStep["nextPC"].toVariant().toUInt();
        cycle->instruction = Decoder::getInstructionDecoded(jsonStep["instruction"].toVariant().toUInt());
        cycle->rf_readAddress1 = jsonStep["rf_readAddress1"].toVariant().toUInt();
        cycle->rf_readAddress2 = jsonStep["rf_readAddress2"].toVariant().toUInt();
        cycle->rf_writeAddress = jsonStep["rf_writeAddress"].toVariant().toUInt();
        cycle->rf_dataIn = jsonStep["rf_dataIn"].toVariant().toUInt();
        cycle->rf_dataOut1 = jsonStep["rf_dataOut1"].toVariant().toUInt();
        cycle->rf_dataOut2 = jsonStep["rf_dataOut2"].toVariant().toUInt();
        cycle->alu_op = jsonStep["aluOp"].toVariant().toUInt();
        cycle->alu_in1 = jsonStep["aluIn1"].toVariant().toUInt();
        cycle->alu_in2 = jsonStep["aluIn2"].toVariant().toUInt();
        cycle->alu_zero = jsonStep["aluZero"].toBool();
        cycle->dataOut = jsonStep["memDataOut"].toVariant().toUInt();
        cycle->dvc = jsonStep["dvc"].toBool();
        cycle->branch = jsonStep["branch"].toBool();
        cycle->jr = jsonStep["jr"].toBool();
        cycle->jal = jsonStep["jal"].toBool();
        cycle->dvi = jsonStep["dvi"].toBool();
        cycle->regDst = jsonStep["regDst"].toBool();
        cycle->memRead = jsonStep["memRead"].toBool();
        cycle->memToReg = jsonStep["memToReg"].toBool();
        cycle->memWrite = jsonStep["memWrite"].toBool();
        cycle->aluSrc = jsonStep["aluSrc"].toBool();
        cycle->regWrite = jsonStep["regWrite"].toBool();
        QJsonArray jsonRegFile = jsonStep["RegFile"].toArray();
        for( int x = 0; x < jsonRegFile.size(); x++ ) {
            QJsonObject jsonReg = jsonRegFile.at(x).toObject();
            cycle->rf_registers[x] = jsonReg["value"].toVariant().toUInt();
        }
        QJsonArray jsonDataMem = jsonStep["DataMem"].toArray();
        for( int x = 0; x < jsonDataMem.size(); x++ ) {
            QJsonObject jsonData = jsonDataMem.at(x).toObject();
            cycle->insert( jsonData["address"].toVariant().toUInt(), jsonData["value"].toVariant().toUInt() );
        }
        newSteps->push_back(cycle);
    }
    this->updateSteps(newSteps);
}
