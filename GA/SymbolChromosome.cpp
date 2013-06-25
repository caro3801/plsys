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


SymbolChromosome::SymbolChromosome():FixedChromosome()
{
}

SymbolChromosome::SymbolChromosome(int nbSymbol, vector<Symbol*> &symbolV, IntDomain* tsNameDomain, std::vector<Symbol*> &tsmap  , bool pOnSym)
    :FixedChromosome(nbSymbol){


    this->mOnSym=pOnSym;
    int opened=0;
    Random *lRandom=NULL;
    lRandom->getInstance();

    QHash<Symbol *,int> symtable;
    for (int i=0;i<tsmap.size();++i){
        symtable.insert(tsmap.at(i),i);
    }

    SymbolFactory *sf=new SymbolFactory();
    for (int i=0; i<nbSymbol;i++){
        Symbol * c=symbolV.at(i);
        IntGene* lGene;
        if (mOnSym){
            lGene=new IntGene(tsNameDomain,symtable.value(c));
        }else{


                char k;
                lGene=new IntGene(tsNameDomain);
                do {
                    lGene->mutate();
                    k=symtable.key(lGene->get())->getName();
                }while((k==']' && opened==0));
                symbolV.at(i)=sf->create(k,c->get(0));


            if(symbolV.at(i)->getName()=='['){
                opened++;
            }if(symbolV.at(i)->getName()==']'){
                opened--;
            }
        }
        this->addGene(lGene);
    }

    delete sf;
    this->mType = Chromosome::eComposite;
    this->mCrossoverMode = eUniform;

}

Chromosome* SymbolChromosome::clone()
{
    SymbolChromosome* lChromosome = new SymbolChromosome();

    lChromosome->mType = this->mType;
    lChromosome->mCrossoverMode = this->mCrossoverMode;

    lChromosome->mNbGenes = this->mNbGenes;
    lChromosome->mGenesArray.clear();


    return lChromosome;
}

Chromosome* SymbolChromosome::copy()
{
    SymbolChromosome* lChromosome = new SymbolChromosome();

    lChromosome->mType = this->mType;
    lChromosome->mOnSym = this->mOnSym;
    lChromosome->mCrossoverMode = this->mCrossoverMode;
    lChromosome->mNbGenes = this->mNbGenes;

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
    (this->mGenesArray[(int)(lRandom->getDouble()*this->mNbGenes)])->mutate();
}



Chromosome* SymbolChromosome::create()
{
    Random* lRandom = NULL;
    lRandom->getInstance();

    SymbolChromosome* lChromosome = new SymbolChromosome();

    lChromosome->mType = this->mType;
    lChromosome->mOnSym = this->mOnSym;
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


Chromosome** SymbolChromosome::cross(Chromosome* pParent, int pPoint){

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

