#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"
#include "QString"
#include "QToolTip"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //4cells
    this->on_action4_triggered();

    mutRate=0.10;
    nbCross=nbCells/3;
    updateGeneticOperators();
    isInit=false;
    isSeeded=false;
    iter=0;
    mPlsys= 0;
    slotsinitialisation();
    initPLSys();


}
void MainWindow::slotsinitialisation(){

   // connect(ui->spinBoxNbCells, SIGNAL(valueChanged(int)), this, SLOT(setNbCells(int)));

    //connect(ui->pushButtonInitPLSys, SIGNAL(clicked(bool)), this, SLOT(initPLSys()));

    connect(ui->pushButtonSeedAG, SIGNAL(clicked(bool)), this, SLOT(seedAG()));
    connect(ui->pushButtonEvolveAG, SIGNAL(clicked(bool)), this, SLOT(newgeneration()));

    //maj mutation rate et cross rate
    connect(ui->horizontalSliderCrossover, SIGNAL(valueChanged(int)),this,SLOT(setCrossoverRate(int)));
    connect(ui->horizontalSliderMutation, SIGNAL(valueChanged(int)),this,SLOT(setMutationRate(int)));

    //orientation pour le plsys
   /* connect(ui->horizontalSliderX, SIGNAL(valueChanged(int)), ui->widgetPLSys, SLOT(setXRotation(int)));
    connect(ui->widgetPLSys, SIGNAL(xRotationChanged(int)),ui->horizontalSliderX , SLOT(setValue(int)));
    connect(ui->horizontalSliderY, SIGNAL(valueChanged(int)), ui->widgetPLSys, SLOT(setYRotation(int)));
    connect(ui->widgetPLSys, SIGNAL(yRotationChanged(int)), ui->horizontalSliderY, SLOT(setValue(int)));
    connect(ui->horizontalSliderZ, SIGNAL(valueChanged(int)), ui->widgetPLSys, SLOT(setZRotation(int)));
    connect(ui->widgetPLSys, SIGNAL(zRotationChanged(int)), ui->horizontalSliderZ, SLOT(setValue(int)));*/

    connect(ui->spinBoxIteration, SIGNAL(valueChanged(int)), this, SLOT(iteration(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initPLSys(){

    ui->widgetPLSys->clear();
    ui->spinBoxGeneration->setValue(0);
    iter=ui->spinBoxIteration->value();
    ui->spinBoxIteration->setEnabled(true);

    mPlsys= initglsystem(iter);
    isInit=true;
    isSeeded=false;
    updatePLSysWidget();
    //updateCells();
    ui->pushButtonSeedAG->setEnabled(true);
    ui->spinBoxGeneration->setEnabled(true);
    ui->labelMutRate->setEnabled(true);
    ui->horizontalSliderMutation->setEnabled(true);
    ui->labelCrossRate->setEnabled(true);
    ui->horizontalSliderCrossover->setEnabled(true);
    ui->pushButtonEvolveAG->setEnabled(false);
}


//mise a jour du contenu des cellules
void MainWindow::updateCells(){
    if (isInit && isSeeded){
        for(int i=0; i < nbCells;i++){
            mCells.at(i)->setSymbolV(this->pop->getGenomesSymbolsArray().at(i));
        }
    }
}
void MainWindow::updatePLSysWidget()
{
    ui->widgetPLSys->setSymbolVector(mPlsys->getPllist());
    /*for(int i=0; i < nbCells;i++){
        mCells.at(i)->clear();
    }*/
}

void MainWindow::setNbCells(int val){
    //QToolTip::showText(QCursor::pos(),tr("Nb Individus : ") % QString::number(val));
    //remove widget in glLayout
    foreach (CellPLSys *p, mCells) {
        delete p;
    }
    mCells.clear();

    QLayoutItem *child;
    while ((child = ui->gridLayoutCells->takeAt(0)) != 0) {
        delete child;
    }

    //nbCells=ui->spinBoxNbCells->value();
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
        ui->gridLayoutCells->addLayout(mCells[i]->v,row,col);
        //GLWidget* tmp=mCells.at(i)->getGLWidget();
        /*connect(ui->horizontalSliderX, SIGNAL(valueChanged(int)), tmp, SLOT(setXRotation(int)));
        connect(tmp, SIGNAL(xRotationChanged(int)),ui->horizontalSliderX , SLOT(setValue(int)));
        connect(ui->horizontalSliderY, SIGNAL(valueChanged(int)), tmp, SLOT(setYRotation(int)));
        connect(tmp, SIGNAL(yRotationChanged(int)), ui->horizontalSliderY, SLOT(setValue(int)));
        connect(ui->horizontalSliderZ, SIGNAL(valueChanged(int)), tmp, SLOT(setZRotation(int)));
        connect(tmp, SIGNAL(zRotationChanged(int)), ui->horizontalSliderZ, SLOT(setValue(int)));*/
        //! [0]
    }

    ui->pushButtonEvolveAG->setEnabled(false);
    updateGeneticOperators();
    ui->statusBar->setToolTip("test");

}

void MainWindow::iteration(int iteration){

    if (isInit && this->iter!=iteration){
        mPlsys->setIteration(iteration);
        this->iter=mPlsys->nbIterations;
        this->mPlsys->afficher(this->mPlsys->getPllist(),this->mPlsys->nbIterations);
    }

    updatePLSysWidget();
    ui->pushButtonEvolveAG->setEnabled(false);

}


void MainWindow::setMutationRate(int val){
    this->mutRate=val;
}
void MainWindow::setCrossoverRate(int val){
    this->nbCross=val;
    this->crossRate=(double)nbCross/(double)nbCells;
}

void MainWindow::seedAG(){
    params = new GAParameter("toto");
    params->setSelection(GAParameter::eSimpleSelection);

    params->setCrossoverRate((double)this->crossRate);
    params->setMutationRate((double)this->mutRate/100);
    initGA();
    ui->pushButtonEvolveAG->setEnabled(true);
    ui->spinBoxGeneration->setValue(0);
    isSeeded=true;
    updateCells();
}

void MainWindow::initGA(){
    this->mg = new PLSysGenome(mPlsys);
    this->pop = new SPopulation(mg,this->nbCells,params,this->iter);

    this->pop->updateGAParameter();
    pop->initialize() ;
    pop->setGenomesSymbolsArray();
}


void MainWindow::newgeneration(){
    params->setMutationRate(this->mutRate);
    params->setCrossoverRate(this->crossRate);
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
    ui->spinBoxGeneration->stepUp();
}


void MainWindow::updateGeneticOperators(){
    ui->horizontalSliderMutation->setValue(mutRate*100);
    ui->horizontalSliderCrossover->setMaximum(nbCells);
    ui->horizontalSliderCrossover->setValue(nbCross);
}




void MainWindow::on_action4_triggered()
{
    nbCells=4;
    setNbCells(nbCells);
    statusBar()->showMessage(tr("4 shapes"));
}

void MainWindow::on_action9_triggered()
{
    nbCells=9;
    setNbCells(nbCells);

    statusBar()->showMessage(tr("9 shapes"));
}
void MainWindow::on_action16_triggered()
{
    nbCells=16;
    setNbCells(nbCells);

    statusBar()->showMessage(tr("16 shapes"));
}
