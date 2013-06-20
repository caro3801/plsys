#include "SymbolPChromosome.h"
#include "IntDomain.h"
#include "BitGene.h"
#include "IntGene.h"
#include "Random.h"
#include "turtlesymbol.h"
#include "iostream"
#include "functions.h"
static double modulo(double x, double y)
{
    /*x modulo y*/
    x-=y*std::abs(int(x/y));
    if (x>=0.) return (x);
    else return (x+y);
}

SymbolPChromosome::SymbolPChromosome():FixedChromosome()
{
}


SymbolPChromosome::SymbolPChromosome(int nbSymbol, vector<Symbol*> &symbolV,DoubleDomain* turtleDomain ):FixedChromosome(nbSymbol){

    Random *lRandom=NULL;
    lRandom->getInstance();

    double similarity=1.;
    for (int i=0; i<nbSymbol;i++){


        //double delta=symbolV.at(i)->getMod();
        //delta=similarity*delta;
        double dmin=symbolV.at(i)->getMin();
        double dmax=symbolV.at(i)->getMax();
        double mod=dmax;
        double delta=(dmax-dmin)*30./100.;
        delta*=similarity;
        double symval=symbolV.at(i)->get(0);
        dmin=modulo(symval-delta,mod)+dmin;
        dmax=modulo(symval+delta,mod);
        if (dmin>dmax){
            double tmp=dmin;
            dmin=dmax;
            dmax=tmp;
        }
        turtleDomain->setMin(dmin);
        turtleDomain->setMax(dmax);

        this->addGene(new DoubleGene(turtleDomain));
    }

    this->mType = Chromosome::eComposite;
    this->mCrossoverMode = eUniform;

}

Chromosome* SymbolPChromosome::create()
{
    Random* lRandom = NULL;
    lRandom->getInstance();

    SymbolPChromosome* lChromosome = new SymbolPChromosome();

    lChromosome->mType = this->mType;
    lChromosome->mCrossoverMode = this->mCrossoverMode;

    lChromosome->mGenesArray = vector<Gene*>(this->mGenesArray.size());
    lChromosome->mGenesArray.clear();
    lChromosome->mNbGenes = this->mGenesArray.size();

    for (unsigned int i=0 ; i<lChromosome->mNbGenes ; i++)
    {
        lChromosome->mGenesArray.push_back(this->mGenesArray[i]->create());
    }

    return lChromosome;
}


Chromosome* SymbolPChromosome::clone()
{
    SymbolPChromosome* lChromosome = new SymbolPChromosome();

    lChromosome->mType = this->mType;
    lChromosome->mCrossoverMode = this->mCrossoverMode;

    lChromosome->mNbGenes = this->mNbGenes;
    lChromosome->mGenesArray.clear();


    return lChromosome;
}

Chromosome* SymbolPChromosome::copy()
{
    SymbolPChromosome* lChromosome = new SymbolPChromosome();

    lChromosome->mType = this->mType;
    lChromosome->mCrossoverMode = this->mCrossoverMode;
    lChromosome->mNbGenes = this->mNbGenes;

    lChromosome->mGenesArray.clear();

    for (unsigned int i=0 ; i<lChromosome->mNbGenes ; i++)
    {
        lChromosome->mGenesArray.push_back(this->mGenesArray[i]->copy());
    }

    return lChromosome;
}

bool SymbolPChromosome::equals(Chromosome* pChromo){

    SymbolPChromosome* lChromo = (SymbolPChromosome*)pChromo;

    int lNbGenesThis = this->mNbGenes;	//this->mGenesArray.size();
    int lNbGenesParam = lChromo->mNbGenes; //lChromo->mGenesArray.size();

    if
            ( lNbGenesThis == lNbGenesParam )
    {
        for	(int i=0 ; i<lNbGenesThis ; i++)
        {
            if
                    (!(this->mGenesArray[i])->equals(lChromo->mGenesArray[i]))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}



void SymbolPChromosome::mutate(){
    Random * lRandom = NULL;
    lRandom->getInstance();

    (this->mGenesArray[(int)(lRandom->getDouble()*this->mNbGenes)])->mutate();
}


Chromosome** SymbolPChromosome::cross(Chromosome* pParent, int pPoint){

    Chromosome* lChildChromo0;
    Chromosome* lChildChromo1;
    lChildChromo0 = this->clone();
    lChildChromo1 = this->clone();
int i;
    for (i=0 ; i<pPoint ; i++)
           {

               lChildChromo0->mGenesArray.push_back(this->mGenesArray[i]->copy());
               lChildChromo1->mGenesArray.push_back(pParent->mGenesArray[i]->copy());
           }
           for (i=pPoint ; i<this->mNbGenes ; i++)
           {
               lChildChromo0->mGenesArray.push_back(pParent->mGenesArray[i]->copy());
               lChildChromo1->mGenesArray.push_back(this->mGenesArray[i]->copy());
           }

           Chromosome** lFixedChromos = new Chromosome*[2];
           lFixedChromos[0] = lChildChromo0;
           lFixedChromos[1] = lChildChromo1;

           return lFixedChromos;
}
