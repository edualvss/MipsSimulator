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
#include <QSettings>
#include <QCloseEvent>
#include <QDockWidget>

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
    connect(ui->actionView_simulation_time,SIGNAL(triggered(bool)),this,SIGNAL(viewSimulationTime()));
    connect(ui->actionEnd_Waveform,SIGNAL(triggered(bool)),this,SIGNAL(closeWaveform()));

    QDockWidget* regFileDock = new QDockWidget(tr("Register File"),this);
    regFileDock->setObjectName("regFileDock");
    regFileDock->setWidget(ui->groupRegFile);
    addDockWidget(Qt::RightDockWidgetArea,regFileDock);
    QDockWidget* dataMemoryDock = new QDockWidget(tr("Data Memory"),this);
    dataMemoryDock->setObjectName("dataMemoryDock");
    dataMemoryDock->setWidget(ui->groupDataMemory);
    addDockWidget(Qt::LeftDockWidgetArea,dataMemoryDock);

//    QSettings windowSettings;
//    this->restoreGeometry(windowSettings.value("mainWindowGeometry").toByteArray());
//    this->restoreState(windowSettings.value("mainWindowState").toByteArray());

    ui->menuView->addActions( createPopupMenu()->actions() );

}

MainWindow::~MainWindow() {
#ifdef DEBUG_METHODS
    std::cout << "Destructor MainWindow" << std::endl;
#endif
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::closeEvent" << std::endl;
#endif

    QSettings settings;
    settings.setValue("mainWindowGeometry",this->saveGeometry());
    settings.setValue("mainWindowState",this->saveState());

    event->accept();
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
        QString currentText = regItem->text();
        QString newText = QString("0x%1").arg(cycleInfo->rf_registers[i],8,16,QLatin1Char('0'));
        regItem->setText( newText );
        if( newText != currentText ) {
            ui->tableRegFile->selectRow( i );
            ui->statusbar->showMessage(tr("Register: %1 updated").arg(QString::fromStdString(RegisterFile::REG_NAMES[i])),2000);
        }
    }
    // Update PC
    ui->tableRegFile->item(32,2)->setText( QString("0x%1").arg(cycleInfo->currentPC,8,16,QLatin1Char('0')) );

    // Update Data Memory
    if( ui->tableDataMemory->rowCount() > 0 ) {
        std::map<unsigned int, unsigned int>::iterator it;
        unsigned int i = 0;
        for( it = cycleInfo->dataMemory.begin(); it != cycleInfo->dataMemory.end(); it++, i++ ) {
            QTableWidgetItem* memItem = ui->tableDataMemory->item(i,1);
            QString currentText = memItem->text();
            QString newText = QString("0x%1").arg(it->second,8,16,QLatin1Char('0'));
            memItem->setText( newText );
            if( newText != currentText ) {
                ui->tableDataMemory->selectRow(i);
                ui->statusbar->showMessage(tr("Data memory in address: %1 updated")
                                           .arg(QString("0x%1").arg(it->first,8,16,QLatin1Char('0'))),2000);
            }
        }
    }

    // Update instruction format
    this->updateInstructionFormat(cycleInfo->instruction);

}

void MainWindow::updateInstructionFormat(Instruction *ins) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::updateInstructionFormat" << std::endl;
#endif
    switch(ins->getFormat()) {
        case Instruction::R: {
            // Activate row of R-format
            ui->l_r_FUNCTION->setVisible(true);
            ui->l_r_OP->setVisible(true);
            ui->l_r_RD->setVisible(true);
            ui->l_r_RS->setVisible(true);
            ui->l_r_RT->setVisible(true);
            ui->l_r_SHAMT->setVisible(true);
            ui->l_R_comment->setVisible(true);
            ui->l_R_format->setVisible(true);
            // Deactivate row of I-format
            ui->l_i_ADDRESS_IMMEDIATE->setVisible(false);
            ui->l_i_comment->setVisible(false);
            ui->l_i_format->setVisible(false);
            ui->l_i_OP->setVisible(false);
            ui->l_i_RS->setVisible(false);
            ui->l_i_RT->setVisible(false);
            // Deactivate row of J-format
            ui->l_j_comment->setVisible(false);
            ui->l_j_format->setVisible(false);
            ui->l_j_OP->setVisible(false);
            ui->l_j_TARGETADDRESS->setVisible(false);

            // Setting value in binary
            ui->l_r_FUNCTION->setText(QString("%1").arg(ins->getFunction(),6,2,QLatin1Char('0')));
            ui->l_r_OP->setText(QString("%1").arg(ins->getOp(),6,2,QLatin1Char('0')));
            ui->l_r_RD->setText(QString("%1").arg(ins->getRd(),5,2,QLatin1Char('0')));
            ui->l_r_RS->setText(QString("%1").arg(ins->getRs(),5,2,QLatin1Char('0')));
            ui->l_r_RT->setText(QString("%1").arg(ins->getRt(),5,2,QLatin1Char('0')));
            ui->l_r_SHAMT->setText(QString("%1").arg(ins->getShamt(),5,2,QLatin1Char('0')));

            // Setting tip (values in other base)
            QString tipFunction = tr("Value: %1, 0x%2")
                    .arg(ins->getFunction())
                    .arg(ins->getFunction(),2,16,QLatin1Char('0'));
            QString tipOp = tr("Value: %1, 0x%2")
                    .arg(ins->getOp())
                    .arg(ins->getOp(),2,16,QLatin1Char('0'));
            QString tipRd = tr("Value: %1, 0x%2")
                    .arg(ins->getRd())
                    .arg(ins->getRd(),2,16,QLatin1Char('0'));
            QString tipRs = tr("Value: %1, 0x%2")
                    .arg(ins->getRs())
                    .arg(ins->getRs(),2,16,QLatin1Char('0'));
            QString tipRt = tr("Value: %1, 0x%2")
                    .arg(ins->getRt())
                    .arg(ins->getRt(),2,16,QLatin1Char('0'));
            QString tipShamt = tr("Value: %1, 0x%2")
                    .arg(ins->getShamt())
                    .arg(ins->getShamt(),2,16,QLatin1Char('0'));

            ui->l_r_FUNCTION->setStatusTip(tipFunction);
            ui->l_r_OP->setStatusTip(tipOp);
            ui->l_r_RD->setStatusTip(tipRd);
            ui->l_r_RS->setStatusTip(tipRs);
            ui->l_r_RT->setStatusTip(tipRt);
            ui->l_r_SHAMT->setStatusTip(tipShamt);

            ui->l_r_FUNCTION->setToolTip(tipFunction);
            ui->l_r_OP->setToolTip(tipOp);
            ui->l_r_RD->setToolTip(tipRd);
            ui->l_r_RS->setToolTip(tipRs);
            ui->l_r_RT->setToolTip(tipRt);
            ui->l_r_SHAMT->setToolTip(tipShamt);

            break;
        }
        case Instruction::I: {
            // Deactivate row of R-format
            ui->l_r_FUNCTION->setVisible(false);
            ui->l_r_OP->setVisible(false);
            ui->l_r_RD->setVisible(false);
            ui->l_r_RS->setVisible(false);
            ui->l_r_RT->setVisible(false);
            ui->l_r_SHAMT->setVisible(false);
            ui->l_R_comment->setVisible(false);
            ui->l_R_format->setVisible(false);
            // Activate row of I-format
            ui->l_i_ADDRESS_IMMEDIATE->setVisible(true);
            ui->l_i_comment->setVisible(true);
            ui->l_i_format->setVisible(true);
            ui->l_i_OP->setVisible(true);
            ui->l_i_RS->setVisible(true);
            ui->l_i_RT->setVisible(true);
            // Deactivate row of J-format
            ui->l_j_comment->setVisible(false);
            ui->l_j_format->setVisible(false);
            ui->l_j_OP->setVisible(false);
            ui->l_j_TARGETADDRESS->setVisible(false);

            // Setting value in binary
            ui->l_i_OP->setText(QString("%1").arg(ins->getOp(),6,2,QLatin1Char('0')));
            ui->l_i_ADDRESS_IMMEDIATE->setText(QString("%1").arg(ins->getImed16(),16,2,QLatin1Char('0')));
            ui->l_i_RS->setText(QString("%1").arg(ins->getRs(),5,2,QLatin1Char('0')));
            ui->l_i_RT->setText(QString("%1").arg(ins->getRt(),5,2,QLatin1Char('0')));

            // Setting tip (values in other base)
            QString tipOp = tr("Value: %1, 0x%2")
                    .arg(ins->getOp())
                    .arg(ins->getOp(),2,16,QLatin1Char('0'));
            QString tipRs = tr("Value: %1, 0x%2")
                    .arg(ins->getRs())
                    .arg(ins->getRs(),2,16,QLatin1Char('0'));
            QString tipRt = tr("Value: %1, 0x%2")
                    .arg(ins->getRt())
                    .arg(ins->getRt(),2,16,QLatin1Char('0'));
            QString tipImed16 = tr("Value: %1, 0x%2")
                    .arg(ins->getImed16())
                    .arg(ins->getImed16(),4,16,QLatin1Char('0'));

            ui->l_i_ADDRESS_IMMEDIATE->setStatusTip(tipImed16);
            ui->l_i_OP->setStatusTip(tipOp);
            ui->l_i_RS->setStatusTip(tipRs);
            ui->l_i_RT->setStatusTip(tipRt);

            ui->l_i_ADDRESS_IMMEDIATE->setToolTip(tipImed16);
            ui->l_i_OP->setToolTip(tipOp);
            ui->l_i_RS->setToolTip(tipRs);
            ui->l_i_RT->setToolTip(tipRt);

            break;
        }
        case Instruction::J: {
            // Deactivate row of R-format
            ui->l_r_FUNCTION->setVisible(false);
            ui->l_r_OP->setVisible(false);
            ui->l_r_RD->setVisible(false);
            ui->l_r_RS->setVisible(false);
            ui->l_r_RT->setVisible(false);
            ui->l_r_SHAMT->setVisible(false);
            ui->l_R_comment->setVisible(false);
            ui->l_R_format->setVisible(false);
            // Deactivate row of I-format
            ui->l_i_ADDRESS_IMMEDIATE->setVisible(false);
            ui->l_i_comment->setVisible(false);
            ui->l_i_format->setVisible(false);
            ui->l_i_OP->setVisible(false);
            ui->l_i_RS->setVisible(false);
            ui->l_i_RT->setVisible(false);
            // Activate row of J-format
            ui->l_j_comment->setVisible(true);
            ui->l_j_format->setVisible(true);
            ui->l_j_OP->setVisible(true);
            ui->l_j_TARGETADDRESS->setVisible(true);

            // Setting value in binary
            ui->l_j_OP->setText(QString("%1").arg(ins->getOp(),6,2,QLatin1Char('0')));
            ui->l_j_TARGETADDRESS->setText(QString("%1").arg(ins->getImed26(),26,2,QLatin1Char('0')));

            // Setting tip (values in other base)
            QString tipOp = tr("Value: %1, 0x%2")
                    .arg(ins->getOp())
                    .arg(ins->getOp(),2,16,QLatin1Char('0'));
            QString tipImed26 = tr("Value: %1, 0x%2")
                    .arg(ins->getImed26())
                    .arg(ins->getImed26(),7,16,QLatin1Char('0'));
            ui->l_j_OP->setStatusTip(tipOp);
            ui->l_j_TARGETADDRESS->setStatusTip(tipImed26);

            ui->l_j_OP->setToolTip(tipOp);
            ui->l_j_TARGETADDRESS->setToolTip(tipImed26);

            break;
        }
    }
}

void MainWindow::setEnabledSimulationTime(bool enabled) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::setEnabledSimulationTime" << std::endl;
#endif
    this->ui->actionView_simulation_time->setEnabled(enabled);
}

void MainWindow::showMessageInStatusBar(QString msg, int timeout) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::showMessageInStatusBar" << std::endl;
#endif
    ui->statusbar->showMessage(msg,timeout);
}
