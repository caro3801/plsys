#ifndef SPOPULATION_H
#define SPOPULATION_H

#include "Population.h"
#include "PLSysGenome.h"
#include "symbol.h"
/******************************************************************************
 ****************************** CLASS DEFINITION ******************************
 ******************************************************************************/
class GA_EXPORT SPopulation : public Population
{

    /**************************************************************************
     ***************************** PUBLIC SECTION *****************************
     **************************************************************************/
protected:
    PLSysGenome * mFather;
public :
    SPopulation(PLSysGenome *pGenome, int pSize, GAParameter* pGAParameter, int iter=0);

    /**
     * Construit une Population à artir d'une sauvegarde sous forme de fichier
     *
     * @param pFileName		nom du fichier de sauvegarde
     * @param pGenome		génome de référence nécessaire à la construction du père
     */
     SPopulation(std::string pFileName, Genome* pGenome);

    /**
     * Destructeur
     */
    virtual ~SPopulation();

    /**
     * clears the array of genomes
     */
    void clearGenomeArray();

    void setGenomesSymbolsArray(); // caroline
    std::vector< std::vector<Symbol*> > getGenomesSymbolsArray(); // caroline

    void initFitness();

    void initialize(bool changeSym, bool changeParam);

    void addIndividual();



    /*-----------------------*/
    /* Opérations génétiques */
    /*-----------------------*/

    /**
     * Calcule les notes de tous les individus de la <code>Population</code>.
     */
    void fitnessPopulation();




    /**
     * intialisation des données relatives à la génération de la nouvelle population
     */
    void initGeneration();


    void selectionGA();

    void crossoverSimpleSelection();


    /**
     * mutation des enfants
     */
    void mutation();
    /**
     * passage à la nouvelle génération
     */
    void replacement();

    /**
     * Passage à la génération suivante.
     */
    void generation();

    /*----------------------------------------------------*/


    /*---------*/
    /* Données */
    /*---------*/
public://MODIF
    /** Liste des individus.
  */
    std::vector< std::vector<Symbol*> > mGenomesSymbolsArray;
    std::vector<int> mSelected;
    //Genome** mGenomesArray;
protected:

    int iter;





};
#endif //SPOPULATION_H
