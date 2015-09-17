#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SimulModels/InstructionMemory.h"
#include "SimulModels/DataMemory.h"
#include "SimulModels/RegisterFile.h"
#include "SimulModels/Decoder.h"
#include "SimulModels/ProgramCounter.h"
#include "Model/Instruction.h"
#include "Model/CycleStatus.h"

#include <QFileDialog>
#include <QMessageBox>

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
    connect(ui->actionLoad_Instructions,SIGNAL(triggered(bool)),this,SLOT(openInstructionFile()));
    connect(ui->actionLoad_Data,SIGNAL(triggered(bool)),this,SLOT(openDataFile()));
    connect(ui->actionSimulate,SIGNAL(triggered(bool)),this,SIGNAL(simulate()));
    connect(ui->actionNext_step, SIGNAL(triggered(bool)),this,SIGNAL(nextStep()));
    connect(ui->actionPrevious_Step,SIGNAL(triggered(bool)),this,SIGNAL(previousStep()));

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
        return;
    }
    emit sendDataFile( filename );
}

void MainWindow::showMessage(QString msg) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::showMessage" << std::endl;
#endif

    QMessageBox::information(this,"Info",msg);

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

    clearTable( ui->tableDataMemory );
    ui->tableDataMemory->clearContents();

    ui->tableDataMemory->setRowCount(map.size());
    int i = 0;
    while( it != map.constEnd() ) {
        unsigned int addr = it.key();
        unsigned int val = it.value();
        ui->tableDataMemory->setItem( i, 0, new QTableWidgetItem( QString("0x%1").arg(addr,8,16,QLatin1Char('0')) ) );
        ui->tableDataMemory->setItem( i, 1, new QTableWidgetItem( QString("0x%1").arg(val,8,16,QLatin1Char('0')) ) );
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
        ui->tableInstructionMemory->setItem( i,0, new QTableWidgetItem(QString("0x%1").arg(address,8,16,QLatin1Char('0'))) );
        ui->tableInstructionMemory->setItem( i,1, new QTableWidgetItem( QString("%1").arg(vec[i],8,16,QLatin1Char('0')) ) );
        Instruction* ins = Decoder::getInstructionDecoded(vec[i]);
        ui->tableInstructionMemory->setItem(i,2, new QTableWidgetItem(QString::fromStdString(ins->getFormatedInstruction())));
    }

}

int MainWindow::rowOfPC(unsigned int pc) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::rowOfPC" << std::endl;
#endif

    QString search = QString("0x%1").arg(pc,8,16,QLatin1Char('0'));

    QList<QTableWidgetItem*> items = ui->tableInstructionMemory->findItems(search,Qt::MatchFixedString);
    return items.at(0)->row();
}

void MainWindow::updateStatus(CycleStatus *cycleInfo) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::updateStatus" << std::endl;
#endif

    // Select the current instruction
    int row = this->rowOfPC(cycleInfo->currentPC);
    ui->tableInstructionMemory->selectRow(row);

    // Update register file
    for( unsigned int i = 0; i < 32; i++ ) {
        QTableWidgetItem* regItem = ui->tableRegFile->item(i,2);
        regItem->setText( QString("0x%1").arg(cycleInfo->rf_registers[i],8,16,QLatin1Char('0')) );
    }
    ui->tableRegFile->item(32,2)->setText( QString("0x%1").arg(cycleInfo->currentPC,8,16,QLatin1Char('0')) );

    if( ui->tableDataMemory->rowCount() > 0 ) {
        std::map<unsigned int, unsigned int>::iterator it;
        unsigned int i = 0;
        for( it = cycleInfo->dataMemory.begin(); it != cycleInfo->dataMemory.end(); it++, i++ ) {
            ui->tableDataMemory->item(i,1)->setText( QString("0x%1").arg(it->second,8,16,QLatin1Char('0')) );
        }
    }
}
