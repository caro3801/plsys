#ifndef WINDOW_H
#define WINDOW_H

#include <QVector3D>
#include <QWidget>
#include "CellPLSys.h"
#include "GAParameter.h"
#include "SPopulation.h"
#include "QSpinBox"
class QSpinBox;
class QSlider;
class QLabel;
class QCheckBox;
class QPushButton;
class QGroupBox;
class PLSysGenome;
class SPopulation;
//! [0]

class Window : public QWidget
{
    Q_OBJECT

public:

    Window();
    void initGA();

public slots:
    void setNbCells(int val);
    void initPLSys();
    void newgeneration();
    void iteration(int iter);
    void setCrossoverRate(int val);
    void setMutationRate(int val);
    void seedAG();
    void lsyscreatorwindow();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    void updateCells();

private:
    int nbCells;
    int iter;
    PLSys * mPlsys;
    std::vector<CellPLSys*> mCells;

    QSlider *createVSlider();
    QSlider *createHSlider(int minRange=0, int maxRange=100, int singleStep=1, int pageStep=5, int tickInterval=5);
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;

    QSlider *nbCellsSlider;

    double mutRate;
    QLabel *mutationLabel;
    QSlider *mutationRateSlider;

    double crossRate;
    QLabel *crossoverLabel;
    QSlider *crossoverRateSlider;

    QGroupBox *createSlidersGroup();
    QGroupBox *createCellsGroup();
    QGroupBox *createPLSysGroup();
    QGroupBox *createAGGroup();

    QSpinBox *iterSpinbox;

    QPushButton * createLSysButton;
    QPushButton * initlsysButton;
    bool isInit;
    QPushButton * seedButton;
    bool isSeeded;
    QPushButton * generationButton;
    QGridLayout *glLayout ;

    GAParameter *params;
    PLSysGenome * mg;
    SPopulation * pop;

    PLSys* getPLSys();
};
//! [0]

#endif // WINDOW_H
