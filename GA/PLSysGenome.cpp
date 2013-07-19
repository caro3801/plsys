#include "PLSysGenome.h"
#include "Chromosome.h"
#include "SymbolChromosome.h"
#include "SymbolPChromosome.h"
#include "ColorChromosome.h"

#include "ComplexGene.h"
#include "IntDomain.h"
#include "DoubleDomain.h"
#include "IntGene.h"
#include "DoubleGene.h"

#include "functions.h"
#include <algorithm>
#include "iostream"

using namespace std;

PLSys* PLSysGenome::plsys=initglsystem();
PLSysGenome::PLSysGenome():Genome(1)
{
    plsysInit=false;

}

PLSysGenome::PLSysGenome(int pSize,bool pChangeSym, bool pChangeParam):Genome(3) {
    this->iter=pSize;
    this->changeSym=pChangeSym;
    this->changeParam=pChangeParam;
    DoubleDomain* turtleDomain = new DoubleDomain(0.0f,30.0f);
    this->mTSMap.clear();
    this->mTSMap=this->getPLSys()->mapTS;
    IntDomain* tsNameDomain = new IntDomain(0,this->mTSMap.size()-1);
    vector<Symbol *> symbv=this->getPLSys()->getPllist();
    addChromosome(new SymbolChromosome(symbv.size(),symbv,tsNameDomain,mTSMap,changeSym),1);

    addChromosome(new SymbolPChromosome(symbv.size(),symbv,turtleDomain,changeParam),1);

    addChromosome(new ColorChromosome(2),1);//color
    delete turtleDomain;
    plsysInit=false;
}


PLSysGenome::PLSysGenome(std::vector<Symbol *> symbv): Genome(3){

    DoubleDomain* turtleDomain = new DoubleDomain(0.0f,30.0f);
    for (int i=0;i<(int)symbv.size();i++){

        if(!(std::find(this->mTSMap.begin(), this->mTSMap.end(), symbv[i]) != this->mTSMap.end())) {
            this->mTSMap.push_back(symbv[i]);
        }

    }
    IntDomain* tsNameDomain = new IntDomain(0,this->mTSMap.size()-1);
    addChromosome(new SymbolChromosome(symbv.size(),symbv,tsNameDomain,mTSMap,changeSym), 1);

    addChromosome(new SymbolPChromosome(symbv.size(),symbv,turtleDomain,changeParam),1);
    addChromosome(new ColorChromosome(2),1);//color
    delete turtleDomain;
    plsysInit=false;
}


PLSysGenome::PLSysGenome(int pSize, int nbObjectives):Genome(3, 1) {

    this->iter=pSize;
    this->mTSMap.clear();

    this->mTSMap=this->getPLSys()->mapTS;
    IntDomain* tsNameDomain = new IntDomain(0,this->mTSMap.size()-1);

    DoubleDomain* turtleDomain = new DoubleDomain(0.0f,30.0f);

    vector<Symbol *> symbv=this->plsys->getPllist();

    addChromosome(new SymbolChromosome(symbv.size(),symbv,tsNameDomain,mTSMap,changeSym), 1);

    addChromosome(new SymbolPChromosome(symbv.size(),symbv,turtleDomain,changeParam), 1);
    addChromosome(new ColorChromosome(2),1);//color

    delete turtleDomain;
    plsysInit=false;
}

PLSysGenome::PLSysGenome(PLSys * pPLSys,bool pOnSym,bool pOnParam): Genome(3){

    changeSym=pOnSym;
    changeParam=pOnParam;
    //this->plsys=pPLSys;
    delete plsys;
    setPLSys(*pPLSys);
    this->iter=getPLSys()->nbIterations;
    vector<Symbol *> symbv=getPLSys()->getPllist();

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
    DoubleDomain* turtleDomain = new DoubleDomain(0.0f,30.0f);
    addChromosome(new SymbolChromosome(symbv.size(),symbv,tsNameDomain,mTSMap,changeSym), 1);

    addChromosome(new SymbolPChromosome(symbv.size(),symbv,turtleDomain,changeSym), 1);
    addChromosome(new ColorChromosome(2),1);//color

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
    return new PLSysGenome();
}
Genome* PLSysGenome::create(int iter, bool pChangeSym, bool pChangeParam) {
    return new PLSysGenome(iter,pChangeSym,pChangeParam);
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
    PLSysGenome* lGenome =  new PLSysGenome(this->iter,this->changeParam,this->changeSym);
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
    lGenome->changeParam=this->changeParam;
    lGenome->changeSym=this->changeSym;

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
    int size=(int)((SymbolChromosome*)this->getChromosome(0))->size();
    vector<Symbol *> lsymbolv;
           SymbolFactory *sf=new SymbolFactory();
    for (int i=0;i<size;i++){
        int symbol_intval=((IntGene*)((SymbolChromosome*)this->getChromosome(0))->getGene(i))->get();
        double symbol_doubleval=((DoubleGene*)((SymbolPChromosome*)this->getChromosome(1))->getGene(i))->get();

        lsymbolv.push_back(sf->create(mTSMap.at(symbol_intval)->getName(),symbol_doubleval));

    }
delete sf;

    return lsymbolv;

}

void PLSysGenome::fitness() {
    //the user selection is the fitness

}

void PLSysGenome::fitness(double fitnessVal) {

    this->setNote(fitnessVal);

}

void PLSysGenome::cross(Genome* pGenome, Genome* pOffspringA, Genome* pOffspringB)
{    Random* lRandom = NULL;
     lRandom->getInstance();
      int taille=this->mChromosomesArray[0]->size();
    if (pOffspringA != NULL && pOffspringB != NULL)
    {
        int lPoint = (unsigned int)(lRandom->getDouble()*(taille-1)) + 1;
        for (int i=0 ; i<this->mNbChromosomes ; i++)
        {
            Chromosome** lChromos;

            if (i==0){//sur les symboles pas sur les parametres

                lChromos = ((SymbolChromosome*)this->mChromosomesArray[i])->cross(pGenome->mChromosomesArray[i],lPoint);
                lChromos[0]=checkHooksConsistency(lChromos[0]);
                lChromos[1]=checkHooksConsistency(lChromos[1]);
            }
            if (i==1){
                lChromos = ((SymbolPChromosome*)this->mChromosomesArray[i])->cross(pGenome->mChromosomesArray[i],lPoint);

            }
            if (i==2){//color
                lChromos = this->getChromosome(i)->cross(pGenome->getChromosome(i));

            }
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
        char c=mTSMap.at(((IntGene *)tmpchrom->mGenesArray[i])->get())->getName();
        while (c==']' && opened==0){
            ((IntGene *)tmpchrom->mGenesArray[i])->mutate();
            c=mTSMap.at(((IntGene*)tmpchrom->mGenesArray[i])->get())->getName();
        }
        if(c=='['){
            opened++;
        }
        if(opened>0 && c==']'){
            opened--;
        }
    }
    return tmpchrom;
}


