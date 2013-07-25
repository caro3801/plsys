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

    void newgeneration();
    void updateCells();
    void initPLSys();
    void initGA();
    void seedAG();
    void updateGenome();
    void iteration(int iteration);
public slots:
    void updatePLSysWidget();
    void setNbCells();

private slots:
    void on_action4_triggered();
    void on_action9_triggered();
    void on_action16_triggered();
    void on_horizontalSliderCrossover_valueChanged(int value);
    void on_horizontalSliderMutation_valueChanged(int value);
    void on_pushButtonEvolveAG_clicked();
    void on_pushButtonSeedAG_clicked();
    void on_checkBoxSymbols_clicked(bool checked);
    void on_checkBoxParameters_clicked(bool checked);
    void on_spinBoxIteration_valueChanged(int arg1);
    void on_actionSave_triggered();
};

#endif // MAINWINDOW_H
