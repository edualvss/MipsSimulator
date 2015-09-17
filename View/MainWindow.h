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

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    int rowOfPC(unsigned int pc);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadInstructionMemory(InstructionMemory* );
    void loadDataMemory(DataMemory* );
    void showMessage(QString msg);
    void updateStatus(CycleStatus* );

signals:
    void sendInstructionFile(QString);
    void sendDataFile(QString);
    void simulate();
    void nextStep();
    void previousStep();

private slots:
    void openInstructionFile();
    void openDataFile();

private:
    Ui::MainWindow *ui;
    QString getFile(QString title);
    void clearTable(QTableWidget*);
};

#endif // MAINWINDOW_H
