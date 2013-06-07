#include "SymbolChromosome.h"
#include "IntDomain.h"
#include "BitGene.h"
#include "IntGene.h"
#include "Random.h"
#include "ComplexGene.h"
#include "Chromosome.h"
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
SymbolChromosome::SymbolChromosome():FixedChromosome()
{
}

SymbolChromosome::SymbolChromosome(int nbSymbol, vector<Symbol*> symbolV,DoubleDomain* turtleDomain , IntDomain* tsNameDomain, std::vector<Symbol*> &tsmap  ):FixedChromosome(nbSymbol){


    int opened=0;
    Random *lRandom=NULL;
    lRandom->getInstance();
    for (int i=0; i<nbSymbol;i++){

        Symbol * s=symbolV[i];
        double delta=0;
        double mod=0;
        double dmin=0;
        double dmax=0;
        double symval;
        (s->getNbParams()==0)?symval=lRandom->getDouble()*360.:symval=s->get(0);
        char c=s->getName();

        ComplexGene* lGene= new ComplexGene(2);

            IntGene* tmpGene=new IntGene(tsNameDomain);
            do {
                tmpGene->mutate();
                c=tsmap.at(tmpGene->get())->getName();

            }while((c==']' && opened==0));

            lGene->addGene(tmpGene);

            switch (c){
            case 'f':
                delta=1.;
                mod=5.;
                break;
            case'[':
                opened++;
                delta=30;
                mod=360.;
                break;
            case ']':
                opened--;
                delta=30;
                mod=360.;
                break;
            default:
                delta=10.;
                mod=360.;
                break;
            }

            dmin=modulo(symval-delta,mod);
            dmax=modulo(symval+delta,mod);
            if (dmin>dmax){
                double tmp=dmin;
                dmin=dmax;
                dmax=tmp;
            }

            turtleDomain->setMin(dmin);
            turtleDomain->setMax(dmax);
            lGene->addGene(new DoubleGene(turtleDomain));


        this->addGene(lGene);
    }

    this->mType = Chromosome::eComposite;
    this->mCrossoverMode = eUniform;

}

Chromosome* SymbolChromosome::clone()
{
    SymbolChromosome* lChromosome = new SymbolChromosome();

    lChromosome->mType = this->mType;
    lChromosome->mCrossoverMode = this->mCrossoverMode;

    lChromosome->mNbGenes = this->mNbGenes;
    lChromosome->mGenesArray = vector<Gene*>();
    lChromosome->mGenesArray.clear();


    return lChromosome;
}

Chromosome* SymbolChromosome::copy()
{
    SymbolChromosome* lChromosome = new SymbolChromosome();

    lChromosome->mType = this->mType;
    lChromosome->mCrossoverMode = this->mCrossoverMode;
    lChromosome->mNbGenes = this->mNbGenes;
    lChromosome->mGenesArray = vector<Gene*>();
    lChromosome->mGenesArray.clear();

    for (unsigned int i=0 ; i<lChromosome->mNbGenes ; i++)
    {
        lChromosome->mGenesArray.push_back(this->mGenesArray[i]->copy());
    }

    return lChromosome;
}

bool SymbolChromosome::equals(Chromosome* pChromo){

    SymbolChromosome* lChromo = (SymbolChromosome*)pChromo;

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



void SymbolChromosome::mutate(){
    Random * lRandom = NULL;
    lRandom->getInstance();
    int i = (int)(lRandom->getDouble()*this->mNbGenes);

    (this->mGenesArray[i])->mutate();
}



Chromosome* SymbolChromosome::create()
{
    Random* lRandom = NULL;
    lRandom->getInstance();

    SymbolChromosome* lChromosome = new SymbolChromosome();

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

vector<Symbol *> SymbolChromosome::convertGenesToSymbols(vector<Symbol*> mapTS){

    vector<Symbol *> lsymbolv;
    for (unsigned int i=0;i<this->size();i++){

        ComplexGene* lComplexGene=(ComplexGene*)this->getGene(i);

            int j=((IntGene *)lComplexGene->getGene(0))->get();
            double k=((DoubleGene *)lComplexGene->getGene(1))->get();

            lsymbolv.push_back(new Symbol(mapTS.at(j)->getName(),parametres(1,(param){'x',k})));

    }
    return lsymbolv;
}

