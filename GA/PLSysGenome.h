#ifndef PLSYSGENOME_H
#define PLSYSGENOME_H
#include "Genome.h"
#include "plsys.h"

class PLSysGenome : public Genome
{
public:
    PLSysGenome();
    PLSysGenome(int pSize);
    PLSysGenome(int pSize, int nbObjectives);
    PLSysGenome(std::vector<Symbol *> symbv);
    PLSysGenome(PLSys * pPLSys);

    Genome* clone();
    Genome* copy();
    Genome* create();
    Genome* create(int iter=0);
    Genome* createMoreGeneric(PLSysGenome *pGenome);

    bool equals(Genome* pGenome);
    bool canBeGeneric();
    bool isGeneric();
    virtual void cross(Genome *pGenome, Genome *pOffspringA, Genome *pOffspringB);
    Chromosome * checkHooksConsistency(Chromosome *chrom);
    PLSys *getPLSys();
    void setPLSys(const PLSys &pPLSys);

    void fitness(); // fitness(int fitnessVal) où fitnessVal est la note du tableau
    void fitness(double fitnessVal) ;
    static PLSys *plsys;

    int iter;
    std::vector<Symbol*> mTSMap;
    std::vector<Symbol*> convertSymbolChromosomeToSymbolVector();

    bool plsysInit;
};

#endif // PLSYSGENOME_H
