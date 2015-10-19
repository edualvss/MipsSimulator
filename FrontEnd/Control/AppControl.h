#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>

class MainWindow;
class CycleStatus;
class QProcess;

class AppControl : public QObject {
    Q_OBJECT
private:
    MainWindow* mainWindow;
    QProcess* mipsProcess;
    QString instructionFile;
    QString dataFile;
    std::vector<CycleStatus *>* steps;

    bool simulated;
    bool instructionMemoryLoaded;
    bool dataMemoryLoaded;

    unsigned long int simulationTimeS;
    unsigned long int simulationTimeUS;
    unsigned int step;

    void updateSteps(std::vector<CycleStatus *>* );
    std::vector<unsigned int>* loadFile(QString filename);

    void loadStepsInJson();
    void loadStepsInCSV();

    QMap<unsigned int,unsigned int>* countInstructionAccess();

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
    void showSimulationTime();
    void endSimulation();

};

#endif // CONTROL_H
