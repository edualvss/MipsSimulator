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

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadInstructionMemory(InstructionMemory* );
    void loadDataMemory(DataMemory* );
    void loadRegisterFile(RegisterFile* );

signals:
    void sendInstructionFile(QString);
    void sendDataFile(QString);
    void simulate();

private slots:
    void openInstructionFile();
    void openDataFile();

private:
    Ui::MainWindow *ui;
    QString getFile(QString title);
    void clearTable(QTableWidget*);
};

#endif // MAINWINDOW_H
