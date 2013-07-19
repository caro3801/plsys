#include "ColorChromosome.h"

#include "ComplexGene.h"
#include "IntGene.h"

#include "IntDomain.h"
#include "iostream"
using namespace std;
IntDomain * ColorChromosome::colorDomain = new IntDomain(0,255);

ColorChromosome::ColorChromosome():FixedChromosome()
{
}
ColorChromosome::ColorChromosome(int pSize):FixedChromosome(pSize)
{

    for  (int i=0;i<pSize;i++){
        ComplexGene *lGene= new ComplexGene(3);
        lGene->addGene(new IntGene(ColorChromosome::colorDomain));
        lGene->addGene(new IntGene(ColorChromosome::colorDomain));
        lGene->addGene(new IntGene(ColorChromosome::colorDomain));
        this->addGene(lGene);
        cout<<this->getGene(i)->toString()<<std::endl;
    }

    this->mType = Chromosome::eComposite;
    this->mCrossoverMode = eUniform;
}
