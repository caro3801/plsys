#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "CellPLSys.h"
#include "GAParameter.h"
#include "SPopulation.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void slotsinitialisation();

    void updateGeneticOperators();

private:
    Ui::MainWindow *ui;


    int nbCells;

    std::vector<CellPLSys*> mCells;


    bool isInit;
    PLSys * mPlsys;
    int iter;

    GAParameter *params;
    PLSysGenome * mg;
    SPopulation * pop;

    bool isSeeded;


    double crossRate;
    double mutRate;
    int nbCross;


public slots:
    void initPLSys();
    void iteration(int iteration);
    void updatePLSysWidget();
    void updateCells();
    void setNbCells(int val);
    void setMutationRate(int val);
    void setCrossoverRate(int val);

    void initGA();
    void newgeneration();
    void seedAG();
private slots:
    void on_actionSave_triggered();
};

#endif // MAINWINDOW_H
