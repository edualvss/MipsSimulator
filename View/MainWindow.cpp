#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SimulModels/InstructionMemory.h"
#include "SimulModels/DataMemory.h"
#include "SimulModels/RegisterFile.h"
#include "SimulModels/Decoder.h"
#include "SimulModels/ProgramCounter.h"
#include "Model/Instruction.h"

#include <QFileDialog>

#include <QDebug>

#ifdef DEBUG_METHODS
    #include <iostream>
#endif


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
#ifdef DEBUG_METHODS
    std::cout << "Constructor MainWindow" << std::endl;
#endif

    ui->setupUi(this);

    ui->actionExit->setIcon( this->style()->standardIcon(QStyle::SP_DialogCloseButton) );
    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(close()));

    ui->actionPrevious_Step->setVisible(false);

    connect(ui->actionLoad_Instructions,SIGNAL(triggered(bool)),this,SLOT(openInstructionFile()));
    connect(ui->actionLoad_Data,SIGNAL(triggered(bool)),this,SLOT(openDataFile()));

}

MainWindow::~MainWindow()
{
#ifdef DEBUG_METHODS
    std::cout << "Destructor MainWindow" << std::endl;
#endif
    delete ui;
}

void MainWindow::openInstructionFile() {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::openInstructionFile" << std::endl;
#endif

    QString filename = this->getFile(tr("Select a hexadecimal text instruction file"));

    if( filename.isEmpty() || filename.isNull() ) {
        qDebug() << "Nao";
        return;
    }

    emit sendInstructionFile( filename );
}

void MainWindow::openDataFile() {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::openDataFile" << std::endl;
#endif

    QString filename = this->getFile( tr("Select a hexadecimal text data file") );
    if( filename.isEmpty() || filename.isNull() ) {
        qDebug() << "Nao";
        return;
    }
    emit sendDataFile( filename );
}

void MainWindow::loadRegisterFile(RegisterFile *regFile) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::loadRegisterFile" << std::endl;
#endif

}


QString MainWindow::getFile(QString title) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::getFile" << std::endl;
#endif

    return QFileDialog::getOpenFileName(this,title);

}

void MainWindow::clearTable(QTableWidget *tw) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::clearTable" << std::endl;
#endif

    int rowCount = tw->rowCount();
    int columnCount = tw->columnCount();

    for( int i = 0; i < rowCount; i++ ) {
        for( int x = 0; x < columnCount; x++ ) {
            QWidget* wid = tw->cellWidget(i,x);
            delete wid;
        }
    }
}

void MainWindow::loadDataMemory(DataMemory *dm) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::loadDataMemory" << std::endl;
#endif

    QMap<unsigned int, sc_uint<32> > map(dm->m_DATA);
    QMap<unsigned int, sc_uint<32> >::const_iterator it = map.constBegin();

    dm->debug();

    clearTable( ui->tableDataMemory );
    ui->tableDataMemory->clearContents();

    ui->tableDataMemory->setRowCount(map.size());
    int i = 0;
    while( it != map.constEnd() ) {
        unsigned int addr = it.key();
        unsigned int val = it.value();
        ui->tableDataMemory->setItem( i, 0, new QTableWidgetItem( QString::number(addr,16) ) );
        ui->tableDataMemory->setItem( i, 1, new QTableWidgetItem( QString::number(val,16) ) );
        i++;
        it++;
    }

}

void MainWindow::loadInstructionMemory(InstructionMemory *im) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::loadInstructionMemory" << std::endl;
#endif

    std::vector<sc_uint<32> > vec = im->getInstructions();

    clearTable( ui->tableInstructionMemory );
    ui->tableInstructionMemory->clearContents();

    ui->tableInstructionMemory->setRowCount( vec.size() );

    for(unsigned int i = 0; i < vec.size(); i++ ) {
        unsigned int address = i*4 + INITIAL_PC_ADDRESS;
        ui->tableInstructionMemory->setItem( i,0, new QTableWidgetItem(QString::number(address,16)) );
        ui->tableInstructionMemory->setItem( i,1, new QTableWidgetItem(QString::number(vec[i],16)) );
        Instruction* ins = Decoder::getInstructionDecoded(vec[i]);
        ui->tableInstructionMemory->setItem(i,2, new QTableWidgetItem(QString::fromStdString(ins->getFormatedInstruction())));
    }

}
