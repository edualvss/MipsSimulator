#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Model/Instruction.h"

namespace Ui {
    class MainWindow;
}

class RegisterFile;
class QTableWidget;
class CycleStatus;

class MainWindow : public QMainWindow {
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent*);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadInstructionMemory(std::vector<unsigned int>* instructions);
    void loadDataMemory(std::vector<unsigned int> *data );
    void showMessage(QString msg);
    void showMessageInStatusBar(QString msg,int timeout);
    void updateStatus(CycleStatus* );

    void updateInstructionCounter(QMap<unsigned int, unsigned int>* counter);

    void setEnabledSimulate(bool enabled);
    void setEnabledSimulationTime(bool enabled);
    void resetRegisterFileTable();
signals:
    void sendInstructionFile(QString);
    void sendDataFile(QString);
    void simulate();
    void nextStep();
    void previousStep();
    void viewSimulationTime();

private slots:
    void openInstructionFile();
    void openDataFile();
    void showAbout();

private:
    Ui::MainWindow *ui;
    QString getFile(QString title);
    void clearTable(QTableWidget*);

    // To update status
    int rowOfPC(unsigned int pc);
    void updateInstructionFormat(Instruction* ins);
    void updateDataTable(std::map<unsigned int,unsigned int>* dataMem);
    void updateRegisterFile(unsigned int registers[32]);

    // Auxiliar methods to update view of instruction format
    void updateInstructionRFormat(unsigned int op,unsigned int rs,
                                  unsigned int rt, unsigned rd,
                                  unsigned int shamt,unsigned int function);
    void updateInstructionIFormat(unsigned int op, unsigned int rs,
                                  unsigned int rt, unsigned int imed16);
    void updateInstructionIAddressingMode(Instruction::AddressingMode mode);
    void updateInstructionJFormat(unsigned int op, unsigned int imed26);
    void setEnabledRFormatView(bool);
    void setEnabledIFormatView(bool);
    void setEnabledJFormatView(bool);

};

#endif // MAINWINDOW_H
