#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>

class MainWindow;
class Mips;
class Testbench;

class AppControl : public QObject {
    Q_OBJECT
private:
    MainWindow* mainWindow;
    QString instructionFile;
    QString dataFile;
    Mips* simulator;
    Testbench* tester;
    int clockTime;

    void initSimulator();
    void endSimulator();

public:
    explicit AppControl(QObject *parent = 0);

    void startApp();

signals:

private slots:
    void receiveInstructionFile(QString);
    void receiveDataFile(QString);
    void simulate();
};

#endif // CONTROL_H
