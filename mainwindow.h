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
    bool isSymbols;
    bool isParameters;
    double similarity;


    double crossRate;
    double mutRate;
    int nbCross;


public slots:
    void initPLSys();
    void iteration(int iteration);
    void updatePLSysWidget();
    void updateCells();
    void setNbCells(int val);

    void initGA();
    void newgeneration();
    void seedAG();
private slots:
    void on_action4_triggered();
    void on_action9_triggered();
    void on_action16_triggered();
    void on_checkBoxSymbols_clicked();
    void on_checkBoxParameters_clicked();
    void on_horizontalSliderSimilarity_valueChanged(int value);
    void on_horizontalSliderCrossover_valueChanged(int value);
    void on_horizontalSliderMutation_valueChanged(int value);
};

#endif // MAINWINDOW_H
