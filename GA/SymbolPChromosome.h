#ifndef SYMBOLPCHROMOSOME_H
#define SYMBOLPCHROMOSOME_H
#include "DoubleFixedChromosome.h"
#include "plsys.h"
#include "DoubleDomain.h"
#include "IntGene.h"
#include "DoubleGene.h"
#include "Random.h"
#include "ComplexGene.h"
#include "Chromosome.h"
#include <vector>
using namespace std;
class SymbolPChromosome: public FixedChromosome
{
public:
    bool mOnParam;
    SymbolPChromosome();
    SymbolPChromosome(int nbSymbol, vector<Symbol *> &symbolV, DoubleDomain *turtleDomain, bool pOnParam=true);
    virtual Chromosome* clone();
    virtual Chromosome* copy();
    virtual Chromosome* create();
    virtual bool equals(Chromosome* pChromo);
    virtual void mutate();

    virtual Chromosome** cross(Chromosome *pParent,int pPoint);
};

#endif // SYMBOLPCHROMOSOME_H
