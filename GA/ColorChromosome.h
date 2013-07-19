#ifndef COLORCHROMOSOME_H
#define COLORCHROMOSOME_H
#include "FixedChromosome.h"
#include "IntDomain.h"

class ColorChromosome : public FixedChromosome
{
public:

    static IntDomain * colorDomain;

    ColorChromosome();
    ColorChromosome(int pSize);
};

#endif // COLORCHROMOSOME_H
