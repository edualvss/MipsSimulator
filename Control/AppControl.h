#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>

class MainWindow;
class Mips;
class Testbench;
class CycleStatus;

class AppControl : public QObject {
    Q_OBJECT
private:
    MainWindow* mainWindow;
    QString instructionFile;
    QString dataFile;
    Mips* simulator;
    Testbench* tester;
    std::vector<CycleStatus *>* steps;
    int clockTime;

    bool started;
    bool simulated;
    bool instructionMemoryLoaded;
    bool dataMemoryLoaded;

    unsigned int simulationTime;
    unsigned int step;

    void initSimulator();
    void endSimulator();
    void updateSteps(std::vector<CycleStatus *>* );

public:
    explicit AppControl(QObject *parent = 0);

    void startApp();

signals:

private slots:
    void receiveInstructionFile(QString);
    void receiveDataFile(QString);
    void nextStep();
    void previousStep();
    void simulate();
};

#endif // CONTROL_H
