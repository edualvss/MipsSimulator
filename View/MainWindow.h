#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class InstructionMemory;
class DataMemory;
class RegisterFile;
class QTableWidget;
class CycleStatus;
class Instruction;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    int rowOfPC(unsigned int pc);
    void updateInstructionFormat(Instruction* ins);
protected:
    void closeEvent(QCloseEvent*);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadInstructionMemory(InstructionMemory* );
    void loadDataMemory(DataMemory* );
    void showMessage(QString msg);
    void showMessageInStatusBar(QString msg,int timeout);
    void updateStatus(CycleStatus* );

    void setEnabledSimulationTime(bool enabled);

signals:
    void sendInstructionFile(QString);
    void sendDataFile(QString);
    void simulate();
    void nextStep();
    void previousStep();
    void viewSimulationTime();
    void closeWaveform();

private slots:
    void openInstructionFile();
    void openDataFile();

private:
    Ui::MainWindow *ui;
    QString getFile(QString title);
    void clearTable(QTableWidget*);
};

#endif // MAINWINDOW_H
