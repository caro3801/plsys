#ifndef SYMBOLCHROMOSOME_H
#define SYMBOLCHROMOSOME_H

#include "FixedChromosome.h"
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
    SymbolChromosome(int nbSymbol,vector<Symbol*> symbolV, DoubleDomain *turtleDomain, IntDomain* tsNameDomain, std::vector<Symbol*> &tsmap );

    virtual Chromosome* clone();
    virtual Chromosome* copy();
    virtual Chromosome* create();
    virtual bool equals(Chromosome* pChromo);
    virtual void mutate();
    vector<Symbol *> convertGenesToSymbols(vector<Symbol *> mapTS);

};

#endif // SYMBOLCHROMOSOME_H
