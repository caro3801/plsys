#include "PLSysGenome.h"
#include "Chromosome.h"
#include "SymbolChromosome.h"

#include "Gene.h"
#include "ComplexGene.h"
#include "IntGene.h"
#include "BitGene.h"
#include "IntDomain.h"
#include "IntFixedChromosome.h"
#include "DoubleFixedChromosome.h"
#include "DoubleDomain.h"
#include "DoubleGene.h"

#include "functions.h"
#include "window.h"
#include <algorithm>
#include "iostream"

using namespace std;
PLSys* PLSysGenome::plsys=initglsystem();
PLSysGenome::PLSysGenome():Genome(1)
{
    plsysInit=false;

}

PLSysGenome::PLSysGenome(int pSize):Genome(1) {
    this->iter=pSize;
    DoubleDomain* turtleDomain = new DoubleDomain(0.0f,30.0f);
    this->mTSMap.clear();
    this->mTSMap=this->getPLSys()->mapTS;
    IntDomain* tsNameDomain = new IntDomain(0,this->mTSMap.size()-1);

    addChromosome(new SymbolChromosome(this->getPLSys()->getPllist().size(),this->getPLSys()->getPllist(), turtleDomain,tsNameDomain,mTSMap),1);
    delete turtleDomain;
    plsysInit=false;
}


PLSysGenome::PLSysGenome(std::vector<Symbol *> symbv): Genome(1){

    DoubleDomain* turtleDomain = new DoubleDomain(0.0f,30.0f);
    for (int i=0;i<(int)symbv.size();i++){

        if(!(std::find(this->mTSMap.begin(), this->mTSMap.end(), symbv[i]) != this->mTSMap.end())) {
            this->mTSMap.push_back(symbv[i]);
        }

    }
    IntDomain* tsNameDomain = new IntDomain(0,this->mTSMap.size()-1);
    addChromosome(new SymbolChromosome(symbv.size(),symbv, turtleDomain,tsNameDomain,mTSMap), 1);
    delete turtleDomain;
    plsysInit=false;
}


PLSysGenome::PLSysGenome(int pSize, int nbObjectives):Genome(1, 1) {
    UNUSED(nbObjectives);
    this->iter=pSize;
    DoubleDomain* turtleDomain = new DoubleDomain(0.0f,30.0f);

    this->mTSMap.clear();

    this->mTSMap=this->getPLSys()->mapTS;
    IntDomain* tsNameDomain = new IntDomain(0,this->mTSMap.size()-1);
    addChromosome(new SymbolChromosome(this->plsys->getPllist().size(),this->getPLSys()->getPllist(), turtleDomain,tsNameDomain,mTSMap), 1);

    delete turtleDomain;
    plsysInit=false;
}

PLSysGenome::PLSysGenome(PLSys * pPLSys): Genome(1){

    //this->plsys=pPLSys;
    delete plsys;
    setPLSys(*pPLSys);
    this->iter=getPLSys()->nbIterations;
    vector<Symbol *> symbv=getPLSys()->getPllist();
    DoubleDomain* turtleDomain = new DoubleDomain(0.0f,30.0f);

    foreach (Symbol * s, symbv) {
        if(!(std::find(this->mTSMap.begin(), this->mTSMap.end(), s) != this->mTSMap.end())) {
            this->mTSMap.push_back(s);
        }
    }
    int s=(this->mTSMap.size()>0)?this->mTSMap.size()-1:-1/*erreur*/;
    if (s==-1){
        cout <<"erreur"<<endl;
    }
    IntDomain* tsNameDomain = new IntDomain(0,s);
    addChromosome(new SymbolChromosome(symbv.size(),symbv, turtleDomain,tsNameDomain,mTSMap), 1);

    delete turtleDomain;
    plsysInit=false;
}

PLSys* PLSysGenome::getPLSys(){

    if (!plsysInit){
        plsys->setIteration(this->iter);
        plsysInit=true;
    }

    return this->plsys;
}

void PLSysGenome::setPLSys(const PLSys &pPLSys){
    plsys=new PLSys(pPLSys);
    plsysInit=true;
}
Genome* PLSysGenome::create() {
    return new PLSysGenome(1);
}
Genome* PLSysGenome::create(int iter) {
    return new PLSysGenome(iter);
}

bool PLSysGenome::equals(Genome* pGenome) {
    return this->mChromosomesArray[0]->equals(pGenome->mChromosomesArray[0]);
}

Genome* PLSysGenome::createMoreGeneric(PLSysGenome* pGenome) {
    return pGenome->create(this->iter);
}

bool PLSysGenome::canBeGeneric() {
    return false;
}

bool PLSysGenome::isGeneric() {
    return false;
}

Genome* PLSysGenome::clone() {
    PLSysGenome* lGenome =  new PLSysGenome(this->iter);
    lGenome->iter=this->iter;
    lGenome->mNbChromosomes = this->mNbChromosomes;
    lGenome->mNbNotes = this->mNbNotes;

    lGenome->mNotesArray = new float[this->mNbNotes];
    lGenome->mPopulation = this->mPopulation;
    lGenome->mChromosomesArray = this->mChromosomesArray;
    lGenome->mChromosomesArray.clear();

    return lGenome;
}

Genome* PLSysGenome::copy() {

    PLSysGenome* lGenome =new PLSysGenome();
    lGenome->plsysInit=this->plsysInit;
    lGenome->plsys=this->plsys;
    lGenome->iter=this->iter;
    lGenome->mTSMap=this->mTSMap;

    lGenome->mNbChromosomes=this->mNbChromosomes;
    lGenome->mChromosomesArray=std::vector<Chromosome*>();

    for (int j=0; j<lGenome->mNbChromosomes; j++) {
        lGenome->mChromosomesArray.push_back(this->mChromosomesArray[j]->copy());
    }

    lGenome->mNbNotes = this->mNbNotes;
    lGenome->mNotesArray = new float[this->mNbNotes];

    for (int i=0; i<lGenome->mNbNotes; i++) {
        if (this->mNotesArray[i]) {
            lGenome->mNotesArray[i] = this->mNotesArray[i];
        }
    }
    lGenome->mPopulation=this->mPopulation;

    return lGenome;
}

vector<Symbol*> PLSysGenome::convertSymbolChromosomeToSymbolVector(){
    return ((SymbolChromosome*)this->getChromosome(0))->convertGenesToSymbols(mTSMap);

}

void PLSysGenome::fitness() {
    int ch=0;
    // int s=   this->mChromosomesArray[0]->size() ;
    /*vector<Symbol*> lsymv=this->convertSymbolChromosomeToSymbolVector();
    foreach(Symbol * s,lsymv){
        s->afficher();
    }*/

    cout<< endl;

    cout <<"do you like this chromosome?"<< endl;
    cin >> ch;

    this->setNote((float)(ch)/10.);

}

void PLSysGenome::fitness(double fitnessVal) {

    this->setNote(fitnessVal);

}

void PLSysGenome::cross(Genome* pGenome, Genome* pOffspringA, Genome* pOffspringB)
{
    if (pOffspringA != NULL && pOffspringB != NULL)
    {
        for (int i=0 ; i<this->mNbChromosomes ; i++)
        {
            Chromosome** lChromos = (this->mChromosomesArray[i])->cross(pGenome->mChromosomesArray[i]);
            lChromos[0]=checkHooksConsistency(lChromos[0]);
            lChromos[1]=checkHooksConsistency(lChromos[1]);
            pOffspringA->mChromosomesArray.push_back(lChromos[0]);
            pOffspringB->mChromosomesArray.push_back(lChromos[1]);

            lChromos[0] = NULL;
            lChromos[1] = NULL;
            delete[] lChromos;
        }
    }
}

Chromosome *  PLSysGenome::checkHooksConsistency(Chromosome *chrom){
    Chromosome * tmpchrom=chrom;
    int opened=0;

    for(unsigned int i=0;i<tmpchrom->mNbGenes;i++){
        char c=mTSMap.at(((IntGene*)((ComplexGene *)tmpchrom->mGenesArray[i])->getGene(0))->get())->getName();
        if(c==']'){
            do{
                ((ComplexGene *)tmpchrom->mGenesArray[i])->getGene(0)->mutate();
                c=mTSMap.at(((IntGene*)((ComplexGene *)tmpchrom->mGenesArray[i])->getGene(0))->get())->getName();
            }while(c==']' && opened==0);
            if(opened>0){
                opened--;
            }
        }
        if(c=='['){
            opened++;
        }


    }
    return tmpchrom;
}


