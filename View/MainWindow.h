#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class RegisterFile;
class QTableWidget;
class CycleStatus;
class Instruction;

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

    void setEnabledSimulate(bool enabled);
    void setEnabledSimulationTime(bool enabled);

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

private:
    Ui::MainWindow *ui;
    QString getFile(QString title);
    void clearTable(QTableWidget*);
    int rowOfPC(unsigned int pc);
    void updateInstructionFormat(Instruction* ins);
};

#endif // MAINWINDOW_H