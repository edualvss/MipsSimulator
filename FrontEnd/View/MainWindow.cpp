#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AboutWindow.h"

#include "Model/CycleStatus.h"
#include "Model/Decoder.h"
#include "Model/RegisterFile.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>
#include <QDockWidget>

#ifdef DEBUG_METHODS
    #include <iostream>
#endif

#include <QDebug>

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
    connect(ui->actionAbout,SIGNAL(triggered(bool)),this,SLOT(showAbout()));


    QDockWidget* dataMemoryDock = new QDockWidget(tr("Data Memory"),this);
    dataMemoryDock->setObjectName("dataMemoryDock");
    dataMemoryDock->setWidget(ui->groupDataMemory);
    addDockWidget(Qt::LeftDockWidgetArea,dataMemoryDock);
    QDockWidget* regFileDock = new QDockWidget(tr("Register File"),this);
    regFileDock->setObjectName("regFileDock");
    regFileDock->setWidget(ui->groupRegFile);
    addDockWidget(Qt::RightDockWidgetArea,regFileDock);

    QSettings windowSettings;
    this->restoreGeometry(windowSettings.value("mainWindowGeometry").toByteArray());
    this->restoreState(windowSettings.value("mainWindowState").toByteArray());

    ui->menuView->addActions( createPopupMenu()->actions() );

    this->setAttribute(Qt::WA_AlwaysShowToolTips,true);

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

void MainWindow::resetRegisterFileTable() {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::resetRegisterFileTable" << std::endl;
#endif
    for( int i = 0; i < ui->tableRegFile->rowCount(); i++ ) {
        ui->tableRegFile->item(i,2)->setText( QString("0x00000000") );
    }
    ui->tableRegFile->item(28,2)->setText( QString("0x10008000") ); // $gp
    ui->tableRegFile->item(29,2)->setText( QString("0x7fffeffc") ); // $sp
    ui->tableRegFile->item(32,2)->setText( QString("0x00400000") ); // pc

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
            QTableWidgetItem* item = tw->item(i,x);
            delete wid;
            delete item;
        }
    }
}

void MainWindow::loadDataMemory(std::vector<unsigned int> *data) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::loadDataMemory" << std::endl;
#endif

    clearTable( ui->tableDataMemory );
    ui->tableDataMemory->clearContents();

    ui->tableDataMemory->setRowCount(data->size());
    for( unsigned int i = 0; i < data->size(); i++ ) {
        unsigned int address = i*4 + 0x10010000;
        unsigned int value = data->at(i);
        QTableWidgetItem* itAddress = new QTableWidgetItem( QString("0x%1").arg(address,8,16,QLatin1Char('0')) );
        QString tipAddress = tr("Value: %1, 0b%2")
                .arg( address )
                .arg( address ,32,2,QLatin1Char('0'));
        itAddress->setStatusTip( tipAddress );
        itAddress->setToolTip( tipAddress );
        QTableWidgetItem* itValue = new QTableWidgetItem( QString("0x%1").arg(value,8,16,QLatin1Char('0')) );
        QString tipValue = tr("Value: %1, 0b%2")
                .arg( value )
                .arg( value ,32,2,QLatin1Char('0'));
        itValue->setStatusTip(tipValue);
        itValue->setToolTip(tipValue);
        ui->tableDataMemory->setItem( i, 0, itAddress );
        ui->tableDataMemory->setItem( i, 1, itValue );
    }

}

void MainWindow::loadInstructionMemory(std::vector<unsigned int> *instructions) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::loadInstructionMemory" << std::endl;
#endif

    clearTable( ui->tableInstructionMemory );
    ui->tableInstructionMemory->clearContents();

    ui->tableInstructionMemory->setRowCount( instructions->size() );

    for(unsigned int i = 0; i < instructions->size(); i++ ) {
        unsigned int address = i*4 + 0x00400000;
        unsigned int instruction = instructions->at(i);
        ui->tableInstructionMemory->setItem( i,0, new QTableWidgetItem(QString("0x%1").arg(address,8,16,QLatin1Char('0'))) );
        ui->tableInstructionMemory->setItem( i,1, new QTableWidgetItem( QString("%1").arg(instruction,8,16,QLatin1Char('0')) ) );
        Instruction* ins = Decoder::getInstructionDecoded(instruction);
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
    this->updateRegisterFile(cycleInfo->rf_registers);

    // Update PC
    ui->tableRegFile->item(32,2)->setText( QString("0x%1").arg(cycleInfo->currentPC,8,16,QLatin1Char('0')) );

    // Update Data Memory
    this->updateDataTable(&cycleInfo->dataMemory);

    // Update instruction format
    this->updateInstructionFormat(cycleInfo->instruction);

}

void MainWindow::updateRegisterFile(unsigned int registers[32]) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::updateRegisterFile" << std::endl;
#endif
    for( unsigned int i = 0; i < 32; i++ ) {
        QTableWidgetItem* regItem = ui->tableRegFile->item(i,2);
        QString currentText = regItem->text();
        QString newText = QString("0x%1").arg(registers[i],8,16,QLatin1Char('0'));
        QString tipValue = tr("Value: %1, 0b%2")
                .arg( registers[i] )
                .arg( registers[i] ,32,2,QLatin1Char('0'));
        regItem->setText( newText );
        regItem->setStatusTip( tipValue );
        regItem->setToolTip( tipValue );
        if( newText != currentText ) {
            ui->tableRegFile->selectRow( i );
            ui->statusbar->showMessage(tr("Register: %1 updated").arg(QString::fromStdString(RegisterFile::REG_NAMES[i])),2000);
        }
    }

}

void MainWindow::updateDataTable(std::map<unsigned int, unsigned int> *dataMem) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::updateDataTable" << std::endl;
#endif

    if( dataMem->empty() ) {
        return;
    }

    std::map<unsigned int, unsigned int>::iterator it;
    for( it = dataMem->begin(); it != dataMem->end(); it++ ) {
        unsigned int address = it->first;
        unsigned int value = it->second;
        QString search = QString("0x%1").arg(address,8,16,QLatin1Char('0'));
        QList<QTableWidgetItem*> items = ui->tableDataMemory->findItems(search,Qt::MatchFixedString);
        if( items.size() > 0 ) { // If item already exists
            int line = items.at(0)->row();
            QTableWidgetItem* memDataItem = ui->tableDataMemory->item(line,1);
            QString currentText = memDataItem->text();
            QString newDataText = QString("0x%1").arg(it->second,8,16,QLatin1Char('0'));
            memDataItem->setText( newDataText );
            if( newDataText != currentText ) {
                ui->tableDataMemory->selectRow(line);
                ui->statusbar->showMessage(tr("Data memory in address: %1 updated")
                                           .arg(QString("0x%1").arg(value,8,16,QLatin1Char('0'))),2000);
            }
        } else { // If item do not exists, so create it
            QTableWidgetItem* itAddress = new QTableWidgetItem( QString("0x%1").arg(address,8,16,QLatin1Char('0')) );
            QString tipAddress = tr("Value: %1, 0b%2")
                    .arg( address )
                    .arg( address ,32,2,QLatin1Char('0'));
            itAddress->setStatusTip( tipAddress );
            itAddress->setToolTip( tipAddress );
            QTableWidgetItem* itValue = new QTableWidgetItem( QString("0x%1").arg(value,8,16,QLatin1Char('0')) );
            QString tipValue = tr("Value: %1, 0b%2")
                    .arg( value )
                    .arg( value ,32,2,QLatin1Char('0'));
            itValue->setStatusTip(tipValue);
            itValue->setToolTip(tipValue);
            int rowCount = ui->tableDataMemory->rowCount();
            ui->tableDataMemory->setRowCount( rowCount + 1 );
            ui->tableDataMemory->setItem( rowCount, 0, itAddress );
            ui->tableDataMemory->setItem( rowCount, 1, itValue );
        }
    }
}

void MainWindow::updateInstructionFormat(Instruction *ins) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::updateInstructionFormat" << std::endl;
#endif
    switch(ins->getFormat()) {
        case Instruction::R: {
            // Activate row of R-format
            this->setEnabledRFormatView(true);
            // Deactivate row of I-format
            this->setEnabledIFormatView(false);
            // Deactivate row of J-format
            this->setEnabledJFormatView(false);

            // Update R format
            this->updateInstructionRFormat(ins->getOp(),ins->getRs(),ins->getRt(),
                                           ins->getRd(),ins->getShamt(),ins->getFunction());
            break;
        }
        case Instruction::I: {
            // Deactivate row of R-format
            this->setEnabledRFormatView(false);
            // Activate row of I-format
            this->setEnabledIFormatView(true);
            // Deactivate row of J-format
            this->setEnabledJFormatView(false);

            // Update I format
            this->updateInstructionIFormat(ins->getOp(),ins->getRs(),ins->getRt(),ins->getImed16());
            this->updateInstructionIAddressingMode( ins->getAddressingMode() );

            break;
        }
        case Instruction::J: {
            // Deactivate row of R-format
            this->setEnabledRFormatView(false);
            // Deactivate row of I-format
            this->setEnabledIFormatView(false);
            // Activate row of J-format
            this->setEnabledJFormatView(true);

            // Update J format
            this->updateInstructionJFormat(ins->getOp(),ins->getImed26());
            break;
        }
    }
}

void MainWindow::updateInstructionRFormat(unsigned int op, unsigned int rs,
                                          unsigned int rt, unsigned rd,
                                          unsigned int shamt, unsigned int function) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::updateInstructionRFormat" << std::endl;
#endif
    // Setting value in binary
    ui->l_r_OP->setText(QString("%1").arg(op,6,2,QLatin1Char('0')));
    ui->l_r_RS->setText(QString("%1").arg(rs,5,2,QLatin1Char('0')));
    ui->l_r_RT->setText(QString("%1").arg(rt,5,2,QLatin1Char('0')));
    ui->l_r_RD->setText(QString("%1").arg(rd,5,2,QLatin1Char('0')));
    ui->l_r_SHAMT->setText(QString("%1").arg(shamt,5,2,QLatin1Char('0')));
    ui->l_r_FUNCTION->setText(QString("%1").arg(function,6,2,QLatin1Char('0')));

    // Setting tip (values in other base)
    QString tipOp = tr("Value: %1, 0x%2")
            .arg(op)
            .arg(op,2,16,QLatin1Char('0'));
    QString tipRs = tr("Value: %1, 0x%2")
            .arg(rs)
            .arg(rs,2,16,QLatin1Char('0'));
    QString tipRt = tr("Value: %1, 0x%2")
            .arg(rt)
            .arg(rt,2,16,QLatin1Char('0'));
    QString tipRd = tr("Value: %1, 0x%2")
            .arg(rd)
            .arg(rd,2,16,QLatin1Char('0'));
    QString tipShamt = tr("Value: %1, 0x%2")
            .arg(shamt)
            .arg(shamt,2,16,QLatin1Char('0'));
    QString tipFunction = tr("Value: %1, 0x%2")
            .arg(function)
            .arg(function,2,16,QLatin1Char('0'));

    ui->l_r_OP->setStatusTip(tipOp);
    ui->l_r_RS->setStatusTip(tipRs);
    ui->l_r_RT->setStatusTip(tipRt);
    ui->l_r_RD->setStatusTip(tipRd);
    ui->l_r_SHAMT->setStatusTip(tipShamt);
    ui->l_r_FUNCTION->setStatusTip(tipFunction);

    ui->l_r_OP->setToolTip(tipOp);
    ui->l_r_RS->setToolTip(tipRs);
    ui->l_r_RT->setToolTip(tipRt);
    ui->l_r_RD->setToolTip(tipRd);
    ui->l_r_SHAMT->setToolTip(tipShamt);
    ui->l_r_FUNCTION->setToolTip(tipFunction);
}

void MainWindow::updateInstructionIFormat(unsigned int op, unsigned int rs,
                                          unsigned int rt, unsigned int imed16) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::updateInstructionIFormat" << std::endl;
#endif

    // Setting value in binary
    ui->l_i_OP->setText(QString("%1").arg(op,6,2,QLatin1Char('0')));
    ui->l_i_RS->setText(QString("%1").arg(rs,5,2,QLatin1Char('0')));
    ui->l_i_RT->setText(QString("%1").arg(rt,5,2,QLatin1Char('0')));
    ui->l_i_ADDRESS_IMMEDIATE->setText(QString("%1").arg(imed16,16,2,QLatin1Char('0')));

    // Setting tip (values in other base)
    QString tipOp = tr("Value: %1, 0x%2")
            .arg(op)
            .arg(op,2,16,QLatin1Char('0'));
    QString tipRs = tr("Value: %1, 0x%2")
            .arg(rs)
            .arg(rs,2,16,QLatin1Char('0'));
    QString tipRt = tr("Value: %1, 0x%2")
            .arg(rt)
            .arg(rt,2,16,QLatin1Char('0'));
    QString tipImed16 = tr("Value: %1, 0x%2")
            .arg(imed16)
            .arg(imed16,4,16,QLatin1Char('0'));

    ui->l_i_OP->setStatusTip(tipOp);
    ui->l_i_RS->setStatusTip(tipRs);
    ui->l_i_RT->setStatusTip(tipRt);
    ui->l_i_ADDRESS_IMMEDIATE->setStatusTip(tipImed16);

    ui->l_i_OP->setToolTip(tipOp);
    ui->l_i_RS->setToolTip(tipRs);
    ui->l_i_RT->setToolTip(tipRt);
    ui->l_i_ADDRESS_IMMEDIATE->setToolTip(tipImed16);

}

void MainWindow::updateInstructionJFormat(unsigned int op, unsigned int imed26) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::updateInstructionJFormat" << std::endl;
#endif
    // Setting value in binary
    ui->l_j_OP->setText(QString("%1").arg(op,6,2,QLatin1Char('0')));
    ui->l_j_TARGETADDRESS->setText(QString("%1").arg(imed26,26,2,QLatin1Char('0')));

    // Setting tip (values in other base)
    QString tipOp = tr("Value: %1, 0x%2")
            .arg(op)
            .arg(op,2,16,QLatin1Char('0'));
    QString tipImed26 = tr("Value: %1, 0x%2")
            .arg(imed26)
            .arg(imed26,7,16,QLatin1Char('0'));
    ui->l_j_OP->setStatusTip(tipOp);
    ui->l_j_TARGETADDRESS->setStatusTip(tipImed26);

    ui->l_j_OP->setToolTip(tipOp);
    ui->l_j_TARGETADDRESS->setToolTip(tipImed26);

}

void MainWindow::updateInstructionIAddressingMode(Instruction::AddressingMode mode) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::updateInstructionIAddressingMode" << std::endl;
#endif

    switch(mode) {
        case Instruction::Base:
            ui->l_i_AddressingMode->setText(tr("Base"));
            ui->l_i_AddressingMode->setToolTip(tr("<p><span style=\" font-size:11pt; font-style:italic;\">" \
                                                  "Base</span><span style=\" font-size:11pt;\"> or </span>" \
                                                  "<span style=\" font-size:11pt; font-style:italic;\">" \
                                                  "displacement addressing</span><span style=\" font-size:11pt;\">" \
                                                  ", where the operand is at the memory location whose address is the " \
                                                  "sum of a register and a constant in the instruction.</span></p>" \
                                                  "<p><img src=\":/icons/baseAddressing.png\"/></p>"));
            break;
        case Instruction::Immediate:
            ui->l_i_AddressingMode->setText(tr("Immediate"));
            ui->l_i_AddressingMode->setToolTip(tr("<p><span style=\" font-size:11pt; font-style:italic;\">"\
                                                  "Immediate addressing</span><span style=\" font-size:11pt;\">," \
                                                  " where the operand is a constant within the instruction itself." \
                                                  "</span></p><p><img src=\":/icons/immediateAddressing.png\"/></p>"));
            break;
        case Instruction::PC_relative:
            ui->l_i_AddressingMode->setText(tr("PC-relative"));
            ui->l_i_AddressingMode->setToolTip(tr("<p><span style=\" font-size:11pt; font-style:italic;\">" \
                                                  "PC-relative addressing</span><span style=\" font-size:11pt;\">" \
                                                  ", where the branch address is the sum of the PC and a " \
                                                  "constant in the instruction.</span></p>" \
                                                  "<p><img src=\":/icons/pcRelativeAddressing.png\"/></p>"));
            break;
        case Instruction::Register:
        case Instruction::Pseudodirect:break;
    }

}

void MainWindow::setEnabledRFormatView(bool enabled) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::setEnabledRFormatView" << std::endl;
#endif
    ui->l_r_FUNCTION->setVisible(enabled);
    ui->l_r_OP->setVisible(enabled);
    ui->l_r_RD->setVisible(enabled);
    ui->l_r_RS->setVisible(enabled);
    ui->l_r_RT->setVisible(enabled);
    ui->l_r_SHAMT->setVisible(enabled);
    ui->l_R_comment->setVisible(enabled);
    ui->l_R_format->setVisible(enabled);
    ui->l_r_AddressingMode->setVisible(enabled);

}

void MainWindow::setEnabledIFormatView(bool enabled) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::setEnabledIFormatView" << std::endl;
#endif
    ui->l_i_ADDRESS_IMMEDIATE->setVisible(enabled);
    ui->l_i_comment->setVisible(enabled);
    ui->l_i_format->setVisible(enabled);
    ui->l_i_OP->setVisible(enabled);
    ui->l_i_RS->setVisible(enabled);
    ui->l_i_RT->setVisible(enabled);
    ui->l_i_AddressingMode->setVisible(enabled);

}

void MainWindow::setEnabledJFormatView(bool enabled) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::setEnabledJFormatView" << std::endl;
#endif
    ui->l_j_comment->setVisible(enabled);
    ui->l_j_format->setVisible(enabled);
    ui->l_j_OP->setVisible(enabled);
    ui->l_j_TARGETADDRESS->setVisible(enabled);
    ui->l_j_AddressingMode->setVisible(enabled);

}

void MainWindow::setEnabledSimulate(bool enabled) {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::setEnabledSimulate" << std::endl;
#endif
    this->ui->actionSimulate->setEnabled(enabled);
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

void MainWindow::showAbout() {
#ifdef DEBUG_METHODS
    std::cout << "MainWindow::showAbout" << std::endl;
#endif

    AboutWindow about(this);

    about.exec();

}
