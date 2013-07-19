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
    this->on_horizontalSliderCrossover_valueChanged(ui->horizontalSliderCrossover->value());
    this->on_horizontalSliderMutation_valueChanged(ui->horizontalSliderMutation->value());
    this->on_action4_triggered();
    isSymbols=ui->checkBoxSymbols->isChecked();
    isParameters=ui->checkBoxParameters->isChecked();
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
            mCells.at(i)->uncheck();
            mCells.at(i)->setColorV(this->pop->getGenomesColorsArray(i));
            mCells.at(i)->setSymbolV(this->pop->getGenomesSymbolsArray().at(i));
        }
    }
}
void MainWindow::updatePLSysWidget()
{
    ui->widgetPLSys->setSymbolVector(mPlsys->getPllist());
}

void MainWindow::setNbCells(){
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
        mCells.push_back(new CellPLSys(i,vector<Symbol*>(),this));

        if (nbCells%4==0 && nbCells!=4){
            row= i%4;
        }else if(nbCells%3==0){
            row= i%3;
        }else{
            row=i%2;
        }
        if(row ==0 && i!=0) col++;
        ui->gridLayoutCells->addLayout(mCells[i]->v,row,col);
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
    pop->initialize(!ui->checkBoxSymbols->isChecked(),!ui->checkBoxParameters->isChecked()) ;

    pop->setGenomesSymbolsArray();
    pop->setGenomesColorsArray();
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
        this->pop->setGenomesColorsArray();
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
    setNbCells();
    statusBar()->showMessage(tr("4 shapes"));
}

void MainWindow::on_action9_triggered()
{
    nbCells=9;
    setNbCells();

    statusBar()->showMessage(tr("9 shapes"));
}
void MainWindow::on_action16_triggered()
{
    nbCells=16;
    setNbCells();

    statusBar()->showMessage(tr("16 shapes"));
}


void MainWindow::on_horizontalSliderCrossover_valueChanged(int value)
{

    this->nbCross=value;
    this->crossRate=(double)nbCross/(double)nbCells;

    ui->labelCrossRate->setText(QString("%1%2%3%4").arg("Crossovers : ",QString::number(nbCross),"/",QString::number(nbCells)));
    statusBar()->showMessage(QString("%1%2%3%4").arg("Crossovers : ",QString::number(nbCross),"/",QString::number(nbCells)));

}

void MainWindow::on_horizontalSliderMutation_valueChanged(int value)
{

    this->mutRate=value;

    ui->labelMutRate->setText(QString("%1%2%3").arg("Mutation rate: ",QString::number(mutRate),"%"));
    statusBar()->showMessage(QString("%1%2%3").arg("Mutation rate : ",QString::number(mutRate),"%"));
}

void MainWindow::on_pushButtonEvolveAG_clicked()
{

    statusBar()->showMessage(QString("%1").arg("Evolution : running..."));
    newgeneration();
    statusBar()->showMessage(QString("%1").arg("Evolution : finished."));
}

void MainWindow::on_pushButtonSeedAG_clicked()
{
    statusBar()->showMessage(QString("%1").arg("Boot strap population : running..."));
    seedAG();
    statusBar()->showMessage(QString("%1").arg("Boot strap population : finished."));
}


void MainWindow::on_checkBoxSymbols_clicked(bool checked)
{
    bool res1,res2;
    checked?res1=true:res1=false;
    ui->checkBoxParameters->isChecked()?res2=true:res2=false;
    if (res1 && res2){

        statusBar()->showMessage(QString("%1").arg("Evolution on : parameters and symbols."));
    }else if (res1 && !res2){

        statusBar()->showMessage(QString("%1").arg("Evolution on : symbols."));
    }else if (!res1 && res2){

        statusBar()->showMessage(QString("%1").arg("Evolution on : parameters."));
    }else if (!res1 &&!res2){

        ui->checkBoxParameters->setChecked(true);

        statusBar()->showMessage(QString("%1").arg("Evolution on : parameters."));
    }

}



void MainWindow::on_checkBoxParameters_clicked(bool checked)
{
    bool res1,res2;
    checked?res1=true:res1=false;
    ui->checkBoxSymbols->isChecked()?res2=true:res2=false;
    if (res1 && res2){

        statusBar()->showMessage(QString("%1").arg("Evolution on : parameters and symbols."));
    }else if (res1 && !res2){

        statusBar()->showMessage(QString("%1").arg("Evolution on : parameters."));
    }else if (!res1 && res2){

        statusBar()->showMessage(QString("%1").arg("Evolution on : symbols."));
    }else if (!res1 &&!res2){

        ui->checkBoxSymbols->setChecked(true);

        statusBar()->showMessage(QString("%1").arg("Evolution on : symbols."));
    }
}



void MainWindow::on_spinBoxIteration_valueChanged(int arg1)
{
    iteration(arg1);
    statusBar()->showMessage(QString("%1%2").arg("Iteration : ",arg1));
}
