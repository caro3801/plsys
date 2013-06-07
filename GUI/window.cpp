#include <QtGui>
#include <QGroupBox>
#include <QLabel>
#include "QSlider"
#include "QCheckBox"
#include "QPushButton"
#include "QToolTip"
#include "qobject.h"
#include "functions.h"
#include "window.h"

#include "iostream"
#include <string>

#include "GAParameter.h"
#include "Genome.h"
#include "PLSysGenome.h"
#include "IntDomain.h"
#include "IntFixedChromosome.h"
#include "IntGene.h"
#include "Population.h"
//#include "plsyscreator.h"



using namespace std;

/*
 **********************************************************************************
 ******************************** Ctr de la fenetre *******************************
 **********************************************************************************
 */
Window::Window()
{
    nbCells=4;
    mutRate=0.10;
    crossRate=0.5;
    isInit=false;
    isSeeded=false;
    iter=0;
    mPlsys= 0;
    //GA

    glLayout = new QGridLayout();
    generationButton = new QPushButton("generation");

    QGridLayout *mainLayout = new QGridLayout();
    QVBoxLayout * leftside = new QVBoxLayout();
    QGroupBox * slidersgroup=createSlidersGroup();
    QGroupBox * cellsgroup=createCellsGroup();
    this->setNbCells(nbCells);
    connect(nbCellsSlider, SIGNAL(valueChanged(int)), this, SLOT(setNbCells(int) ));
    QGroupBox * plsysgroup=createPLSysGroup();
    QGroupBox * aggroup=createAGGroup();

    leftside->addWidget((QWidget*)cellsgroup);
    leftside->addWidget((QWidget*)slidersgroup);


    mainLayout->addLayout(leftside,0,0);
    mainLayout->addWidget((QWidget*)plsysgroup,1,0);
    mainLayout->addLayout(glLayout,0,1);
    mainLayout->addWidget((QWidget*)aggroup,1,1);
    setLayout(mainLayout);

    setWindowTitle(tr("Go3DEAF"));
}


/*
 **********************************************************************************
 ******************************** Creation des sliders ****************************
 **********************************************************************************
 */

//horizontal
QSlider *Window::createHSlider(int minRange,int maxRange,int singleStep,int pageStep,int tickInterval)
{
    QSlider *slider = new QSlider(Qt::Horizontal);

    slider->setRange(minRange, maxRange);

    slider->setSingleStep(singleStep);
    slider->setPageStep(pageStep);
    slider->setTickInterval(tickInterval);
    slider->setTickPosition(QSlider::TicksAbove);
    return slider;
}

//vertical
QSlider *Window::createVSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 360 * 16);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}
//! [2]

/*
 **********************************************************************************
 *************************** Creation et maj des cellules *************************
 **********************************************************************************
 */

void Window::setNbCells(int val){
    QToolTip::showText(QCursor::pos(),tr("Nb Individus : ") % QString::number(val));
    //remove widget in glLayout
    QLayoutItem *child;
    while ((child = glLayout->takeAt(0)) != 0) {
        delete child;
    }

    mCells.clear();
    nbCells=val;
    int row,col=0;
    for (int i=0;i<nbCells;i++){
        //mCells.push_back(new CellPLSys(i,pop->getGenomesSymbolsArray().at(i)));
        mCells.push_back(new CellPLSys(i,vector<Symbol*>()));

        if (nbCells%4==0 && nbCells!=4){
            row= i%4;
        }else if(nbCells%3==0){
            row= i%3;
        }else{
            row=i%2;
        }
        if(row ==0 && i!=0) col++;
        CellPLSys * tmpC=mCells.at(i);
        glLayout->addLayout(tmpC,row,col);
        GLWidget* tmp=tmpC->getGLWidget();
        connect(this->xSlider, SIGNAL(valueChanged(int)), tmp, SLOT(setXRotation(int)));
        connect(tmp, SIGNAL(xRotationChanged(int)), this->xSlider, SLOT(setValue(int)));
        connect(this->ySlider, SIGNAL(valueChanged(int)), tmp, SLOT(setYRotation(int)));
        connect(tmp, SIGNAL(yRotationChanged(int)), this->ySlider, SLOT(setValue(int)));
        connect(this->zSlider, SIGNAL(valueChanged(int)), tmp, SLOT(setZRotation(int)));
        connect(tmp, SIGNAL(zRotationChanged(int)), this->zSlider, SLOT(setValue(int)));
        //! [0]
    }
    generationButton->setEnabled(false);

}

//mise a jour du contenu des cellules
void Window::updateCells(){
    if (!isInit){
        for(int i=0; i < nbCells;i++){
            mCells.at(i)->setSymbolV(vector<Symbol*>());
        }
    }else if (isInit && !isSeeded){
        for(int i=0; i < nbCells;i++){
            mCells.at(i)->setSymbolV(mPlsys->getPllist());
        }
    }else if (isInit && isSeeded){
        for(int i=0; i < nbCells;i++){
            mCells.at(i)->setSymbolV(this->pop->getGenomesSymbolsArray().at(i));
        }
    }
}

/*
 **********************************************************************************
 *********************************Creation des groupes ****************************
 **********************************************************************************
 */

//sliders rotation
QGroupBox * Window::createSlidersGroup(){
    QGroupBox *groupBox = new QGroupBox( tr("Rotation sliders"));

    xSlider = createVSlider();
    ySlider = createVSlider();
    zSlider = createVSlider();

    xSlider->setValue(15 * 16);
    ySlider->setValue(345 * 16);
    zSlider->setValue(0 * 16);
    QHBoxLayout *rotationLayout = new QHBoxLayout();
    rotationLayout->addWidget(xSlider);
    rotationLayout->addWidget(ySlider);
    rotationLayout->addWidget(zSlider);
    groupBox->setLayout((QLayout *)rotationLayout);


    return groupBox;
}

//cell manager
QGroupBox * Window::createCellsGroup(){

    QGroupBox *groupBox = new QGroupBox(tr("Cells per generation"));

    nbCellsSlider = createHSlider(4,12,2,4,4);
    nbCellsSlider->setValue(4);
    QHBoxLayout *cellItemLayout = new QHBoxLayout();
    cellItemLayout->addWidget(nbCellsSlider);
    groupBox->setLayout(cellItemLayout);
    return groupBox;
}

//plsys manager
QGroupBox * Window::createPLSysGroup(){

    QGroupBox *groupBox = new QGroupBox(tr("Parametric L-system"));
    createLSysButton = new QPushButton("create plsys");
    connect(createLSysButton,SIGNAL(clicked()),this,SLOT(lsyscreatorwindow()));
    initlsysButton = new QPushButton("init plsys");
    iterSpinbox = new QSpinBox();
    iterSpinbox->setEnabled(false);
    connect(iterSpinbox,SIGNAL(valueChanged(int)),this,SLOT(iteration(int)));

    QVBoxLayout *plsysLayout = new QVBoxLayout();
    plsysLayout->addWidget(createLSysButton);
    plsysLayout->addWidget(initlsysButton);
    plsysLayout->addWidget(iterSpinbox);
    groupBox->setLayout(plsysLayout);
    return groupBox;
}

// ag controls
QGroupBox * Window::createAGGroup(){

    QGroupBox *groupBox = new QGroupBox(tr("Evolution controls"));
    generationButton = new QPushButton("generation");
    generationButton->setEnabled(false);

    seedButton = new QPushButton("seed");
    seedButton->setEnabled(false);
    connect(initlsysButton, SIGNAL(clicked(bool)), this, SLOT(initPLSys()));

    mutationRateSlider = createHSlider();
    mutationRateSlider->setValue((int)(mutRate*100));
    mutationLabel = new QLabel(tr("Mutation rate: ")% QString::number(mutationRateSlider->value())% QString("%"));
    connect(mutationRateSlider, SIGNAL(valueChanged(int)), this, SLOT(setMutationRate(int)));


    crossoverRateSlider = createHSlider(0,nbCells,1,1,2);
    crossoverRateSlider->setValue((int)(crossRate*nbCells));
    crossoverLabel = new QLabel(QString("Crossovers per generation : ") % QString::number(crossoverRateSlider->value())% QString("/")% QString::number(nbCells));
    connect(crossoverRateSlider, SIGNAL(valueChanged(int)), this, SLOT(setCrossoverRate(int)));

    connect(seedButton, SIGNAL(clicked(bool)), this, SLOT(seedAG()));
    connect(generationButton, SIGNAL(clicked(bool)), this, SLOT(newgeneration()));


    QGridLayout *agLayout = new QGridLayout();
    agLayout->addWidget(seedButton,0,0);
    agLayout->addWidget(generationButton,1,0);
    agLayout->addWidget(mutationLabel,0,1);
    agLayout->addWidget(mutationRateSlider,0,2);
    agLayout->addWidget(crossoverLabel,1,1);
    agLayout->addWidget(crossoverRateSlider,1,2);
    groupBox->setLayout(agLayout);

    return groupBox;
}




/*
 **********************************************************************************
 *********************************ALGO GENETIQUE *********************************
 **********************************************************************************
 */


void Window::seedAG(){
    params = new GAParameter("toto");
    params->setSelection(GAParameter::eSimpleSelection);
    params->setCrossoverRate((double)crossoverRateSlider->value()/nbCells);
    params->setMutationRate((double)mutationRateSlider->value()/100);
    initGA();
    generationButton->setEnabled(true);
    isSeeded=true;
    updateCells();
}

void Window::initGA(){


    this->mg = new PLSysGenome(mPlsys);
    this->pop = new SPopulation(mg,this->nbCells,params,this->iter);

    this->pop->updateGAParameter();
    pop->initialize() ;
    pop->setGenomesSymbolsArray();
}

/*
 * Operateurs genetiques
 */



void Window::setMutationRate(int val){
    mutationLabel->setText(tr("Mutation rate : ") % QString::number(val) % QString("%"));
    QToolTip::showText(QCursor::pos(),tr("Mutation rate : ") % QString::number(val) % QString("%"));
    if (isSeeded){
        params->setMutationRate(val/100.);
    this->pop->updateGAParameter();
    }
}

void Window::setCrossoverRate(int val){
    crossoverLabel->setText(tr("Crossovers per generation : ") % QString::number(val) % QString("/") % QString::number(nbCells) );

    QToolTip::showText(QCursor::pos(),tr("Crossovers rate : ") % QString::number(val*100/nbCells) % QString("%"));
    if (isSeeded){
        params->setCrossoverRate(val/(double)nbCells);
        this->pop->updateGAParameter();
    }
}

/*
 **********************************************************************************
 ********************************* PLSys (init iter) ******************************
 **********************************************************************************
 */
PLSys* Window::getPLSys(){
    return this->mPlsys;
}

void Window::initPLSys(){
    mPlsys= initglsystem(iter);
    iterSpinbox->setEnabled(true);
    seedButton->setEnabled(true);
    isInit=true;
    isSeeded=false;
    updateCells();
}

void Window::newgeneration(){

    this->pop->mSelected.clear();
    for (int i=0;i<nbCells;i++){
        if (this->mCells[i]->isSelected()){
            this->pop->mSelected.push_back(i);
        }
    }
    if (this->pop->mSelected.size()>1){
        this->pop->generation();
        this->pop->setGenomesSymbolsArray();
        updateCells();
    }

}

void Window::iteration(int iter){
    if (isInit){
        mPlsys->setIteration(iter);
        this->iter=mPlsys->nbIterations;
        this->mPlsys->afficher(this->mPlsys->getPllist(),this->mPlsys->nbIterations);

    }

    if (isSeeded){
        delete mg;
        delete pop;
        initGA();
    }
    updateCells();
}


/*
 **********************************************************************************
 ********************************* Key event (quit, iter) *************************
 **********************************************************************************
 */


void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape){
        delete xSlider;
        delete ySlider;
        delete zSlider;

        mCells.clear();
        QLayoutItem *child;
        while ((child = glLayout->takeAt(0)) != 0) {
            delete child;
        }
        delete glLayout ;
        delete nbCellsSlider;

        delete initlsysButton;
        if (isInit){
            delete mPlsys;
        }
        delete seedButton;
        delete generationButton;
        if (isSeeded){
            delete params;
            delete mg;
            delete pop;
        }

        delete mutationLabel;
        delete mutationRateSlider;
        delete crossoverLabel;
        delete crossoverRateSlider;

        close();
    }
    else if (e->key()== Qt::Key_Space)
    {
        iter++;
        iteration(this->iter);
    }
}


void Window::lsyscreatorwindow(){
    //PLSysCreator * p=new PLSysCreator(this);
    //p->open();
}
