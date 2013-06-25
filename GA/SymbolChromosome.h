#ifndef SYMBOLCHROMOSOME_H
#define SYMBOLCHROMOSOME_H

#include "IntFixedChromosome.h"
#include "plsys.h"
#include "DoubleDomain.h"
#include "IntDomain.h"
#include "DoubleGene.h"
#include "Random.h"
#include "ComplexGene.h"
#include "Chromosome.h"
#include <vector>
using namespace std;
class SymbolChromosome : public FixedChromosome
{
public:
    SymbolChromosome();
    SymbolChromosome(int nbSymbol,vector<Symbol*> &symbolV, IntDomain* tsNameDomain, std::vector<Symbol*> &tsmap, bool pOnSym=true );

    bool mOnSym;
    virtual Chromosome* clone();
    virtual Chromosome* copy();
    virtual Chromosome* create();
    virtual bool equals(Chromosome* pChromo);
    virtual void mutate();
    virtual Chromosome** cross(Chromosome *pParent,int pPoint);

};

#endif // SYMBOLCHROMOSOME_H
