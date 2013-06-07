/*
 *  OOGA_CPP
 *  2001-2009 by StÈéphane Sanchez, Alain Berro, HervéÈ Luga and Yves Duthen
 *
 *
 *  Contact:
 *  Institut de Recherche en Informatique de Toulouse (IRIT)
 *  Equipe Visual Objects : from Reality To EXpression (VORTEX)
 *  Université Toulouse 1 Sciences Sociales (IRIT-UT1)
 *  http://www.irit.fr/-Equipe-VORTEX-
 *
 */
/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/
#include "SPopulation.h"

#include "Random.h"
#include "GAParameter.h"
#include "Genome.h"
#include "Chromosome.h"
#include "IntGene.h"
#include "Gene.h"

#include "Parser.h"
#include "PopulationParser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <queue>
#include <time.h>
#include <vector>
#include <string>
#include <cstdlib>

#include "ComplexGene.h"
#include "PLSysGenome.h"

/******************************************************************************
 ************************ DEFINE AND CONSTANT SECTION  ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/
using namespace std;
/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Construit une Population vide.
 *
 * Ce constructeur intialise le génome de référence, la taille de la
 * Population et les paramétres d'évolution mais il ne crée pas
 * les individus de cette Population.
 *
 * Le <b>génome de référence</b> est le génome qui va servir ‡ créer toute
 * la Population. Chaque individu aura une structure génétique
 * identique au génome de référence.
 *
 * @param	g		Genome de référence
 * @param	taille	taille de la population
 * @param	p		ensemble des paramétres d'évolution
 *
 *****************************************************************************/
SPopulation::SPopulation( PLSysGenome* pGenome, int pSize, GAParameter* pGAParameter,int iter)
    :Population(pGenome,pSize,pGAParameter)
{

    this->mInitialNbGenomes	= pSize;

    this->mGenomesArray = vector<Genome*>(this->mInitialNbGenomes);
    this->iter=iter;
    this->mFather = (PLSysGenome *)pGenome->copy();
    this->mGAParameter	= pGAParameter;
    this->mNbGenomes = 0;
    this->mIsInitialized = false;
    this->mRecordStats = true;

    for (int i=0; i< pSize; i++)
    {
        this->mGenomesArray[i] = NULL;
    }

    this->mStatIndex = 0;
    this->mStatsLength = 10;

    this->mDurations = new long[7];

    this->mMaxStats = new double[this->mStatsLength];
    this->mAverageStats = new double[this->mStatsLength];

    this->mAverageNotes = new double[pGenome->getNbObjectives()+1];

    mStatFile = "AG_Statistics.txt";
    mWriteToStatFile = false;
    mStatFileOpen = false;
}





/******************************************************************************
 * Population::~Population
 ******************************************************************************/
SPopulation::~SPopulation()
{
    std::vector<Genome*>::iterator lItor;

    while ( ! this->mGenomesArray.empty() )
    {
        lItor = this->mGenomesArray.begin();
        if
                ( *lItor != NULL )
        {
            delete *lItor;
            *lItor = NULL;
        }
        this->mGenomesArray.erase(lItor);
    }

    delete this->mFather;
    this->mFather = NULL;

    delete[] this->mDurations;

    delete[] this->mMaxStats;
    delete[] this->mAverageStats;
    delete[] this->mAverageNotes;
}


/******************************************************************************
 * clears the array of genomes
 ******************************************************************************/
void SPopulation::clearGenomeArray()
{
    vector<Genome*>::iterator lItor;

    while
            ( ! this->mGenomesArray.empty() )
    {
        lItor = this->mGenomesArray.begin();
        if
                ( *lItor != NULL )
        {
            delete *lItor;
            *lItor = NULL;
        }
        this->mGenomesArray.erase(lItor);
    }
    this->mNbGenomes=0;
}



/*****************************************************************************
 * Initialise aléatoirement les individus de la population.
 *
 * Cette méthode crée et d'initialise aléatoirement
 * les individus de la Population.
 *
 *****************************************************************************/
void SPopulation::initialize()
{

    int i;

    if (this->mNbGenomes != 0 and this->mInitialNbGenomes < this->mNbGenomes)
    {
        this->mInitialNbGenomes = this->mNbGenomes;
    }


    for (i=this->mNbGenomes ; i<this->mInitialNbGenomes ; i++)
    {
        this->add(((PLSysGenome*)(this->mFather))->create(this->iter) );
    }

    this->mIsInitialized = true;
}

void SPopulation::setGenomesSymbolsArray(){
    this->mGenomesSymbolsArray.clear();
    cout <<"nombre de chromosomes : "<<this->mInitialNbGenomes<<endl;
    for (int i=0 ; i<this->mInitialNbGenomes ; i++)
    {
        vector<Symbol*> lsymv=((PLSysGenome*)(this->mGenomesArray[i]))->convertSymbolChromosomeToSymbolVector();

        cout<< i<< "/ ";
        foreach(Symbol * s,lsymv){
            s->afficher();
        }
        cout<< endl;
        this->mGenomesSymbolsArray.push_back(lsymv);
    }

}

vector< vector<Symbol*> > SPopulation::getGenomesSymbolsArray(){
    return mGenomesSymbolsArray;
}


void SPopulation::initFitness() //initFitness(int[] tableauNoteFitness)
{
    this->mSelected=vector<int>();
    
    if (this->mGAParameter->getNbCPU()==1)
    {
        double ch=0;

        for (int i=0 ; i<this->mInitialNbGenomes ; i++)
        {
            cout <<i<<"/ do you select this chromosome?"<< endl;
            cin >> ch;

            if (ch==1){
                this->mSelected.push_back(i);
            }

        }
    }
    else
    {
        this->mChangesArray = new bool[mNbGenomes];
        for (unsigned int i=0; i<mGenomesArray.size(); i++) {
            this->mChangesArray[i]=true;
        }
        
        fitnessPopulation();
        
        delete[] mChangesArray;
    }
    
    this->statistics();
}




/*****************************************************************************
 * Ajoute un individu ‡ la Population.
 *
 * Cette méthode crée un nouvel individu aléatoirement, l'ajoute ‡ la
 * Population et calcule sa fitness.
 *
 *****************************************************************************/
void SPopulation::addIndividual()
{
    Genome* lGenome = ((PLSysGenome*)(this->mFather))->create(this->iter);

    lGenome->setPopulation(this);

    if
            ( this->mNbGenomes < this->mInitialNbGenomes	)
    {
        this->mGenomesArray[this->mNbGenomes] = lGenome;
        this->mNbGenomes++;
    }
    else
    {
        this->mGenomesArray.push_back(lGenome);
        this->mNbGenomes++;
    }
}





void SPopulation::fitnessPopulation()
{
    this->mSelected.clear();
    // If I have only one CPU, I do sequential evaluation
    if (mGAParameter->getNbCPU()==1)
    {
        double ch=0;

        for (int i=0 ; i<this->mInitialNbGenomes ; i++)
        {
            cout <<i<<"/ do you select this chromosome?"<< endl;
            cin >> ch;


            if (ch==1){
                this->mSelected.push_back(i);
            }

        }

    }

    /*----- Calculs statistiques -----*/

    this->statistics();

    if
            (this->mRecordStats)
    {
        this->mStatIndex = (this->mStatIndex+1) % this->mStatsLength;
        this->mMaxStats[this->mStatIndex] = this->mMaxNote;
        this->mAverageStats[this->mStatIndex] = this->mAverageNotes[0];
    }
}





/******************************************************************************
 * intialisation des données relatives à la génération de la nouvelle population
 ******************************************************************************/
void SPopulation::initGeneration()
{
    this->mOffsprings.clear();

    if
            ( this->mGAParameter->getMethod() == GAParameter::eSSGA )
    {
        this->mMatingSize = mOverlappingSize;
    }
    else
    {
        mMatingSize = mNbGenomes;
    }

    this->mMatingPool = new int[this->mMatingSize];

    this->mOffsprings = vector<Genome*>(this->mInitialNbGenomes);
    for (int i=0; i< this->mInitialNbGenomes; i++)
    {
        this->mOffsprings[i] = NULL;
    }

    this->mSurvived = new bool[this->mNbGenomes];

    this->mChangesArray = new bool[this->mMatingSize];	// Tableau d'états des individus

    for (int i=0; i<this->mNbGenomes; i++)
    {
        mSurvived[i] = false;
    }

    for (int j=0; j<mMatingSize; j++)
    {
        mChangesArray[j] = true;
    }

}


/******************************************************************************
 *
 ******************************************************************************/
void SPopulation::selectionGA()
{
    int i,j,k;

    double* lNotesArray = new double[this->mNbGenomes];	// Tableau des notes des Individus
    double* lShareArray = new double[this->mNbGenomes];	// Tableau des "niche count" des individus
    double* lSumArray = new double[this->mNbGenomes];	// Tableau des sommes pour la roulette pipée

    int* lResultArray = new int[this->mNbGenomes];

    Genome* lAux = NULL;

    Random* lRandom = NULL;
    lRandom->getInstance();

    //----- CGA
    for (i=0 ; i<this->mNbGenomes ; i++)
    {
        lNotesArray[i] = (this->mGenomesArray[i])->getNote();
    }

    // Scaling
    if
            ( this->mScaling )
    {
    }

    // Sharing
    if
            ( this->mSharing )
    {
        this->share(lShareArray);

        for
                (i=0 ; i<this->mNbGenomes ; i++)
        {
            lNotesArray[i] /= lShareArray[i];
        }
    }


    //----- Initialisation des modes de sélection
    switch
            (this->mSelection)
    {
    case 0 : //GAParameter::RANK :
        for (i=0 ; i<this->mNbGenomes ; i++)
        {
            lResultArray[i] = i;
        }

        sortIndividuals(lResultArray, 0, this->mNbGenomes-1, lNotesArray);

        for (i=0 ; i<this->mNbGenomes ; i++)
        {
            lNotesArray[i] = lResultArray[i];
        }
        break;

    case 2 : //GAParameter::ROULETTE_WHEEL :
        if
                ( this->mMinNote < 0.0 )
        {
            //	throw new IllegalArgumentException("Population, generation : la selection par roulette pipee ne peut s'appliquer que sur un ensemble de notes positives.");
        }
        else
        {
            lSumArray[0] = lNotesArray[0];
            for (i=1 ; i<this->mNbGenomes ; i++)
            {
                lSumArray[i] = lSumArray[i-1] + lNotesArray[i];
            }
        }
        break;

    case 3 : //GAParameter::STOCHASTIC_REMAINDER :
        if
                (	this->mMinNote < 0.0	)
        {
            //	throw new IllegalArgumentException("Population, generation : la selection par reste stochastique ne peut s'appliquer que sur un ensemble de notes positives.");
        }
        else
        {
            lSumArray[0] = lNotesArray[0];
            for (i=1 ; i<this->mNbGenomes ; i++)
            {
                lSumArray[i] = lSumArray[i-1] + lNotesArray[i];
            }
            this->mGap = lSumArray[this->mNbGenomes - 1] / this->mNbGenomes;
            this->mMargin = lRandom->getDouble()*this->mGap;
        }
        break;

    default :
        break;
    }

    //----- Sélection : Passage à la population this à mating pool
    i = this->mMatingSize;

    switch(this->mSelection)
    {
    case 0 : //GAParameter::RANK :
    case 2 : //GAParameter::ROULETTE_WHEEL :
        while (i-- > 0)
        {
            mMatingPool[i] = this->rouletteWheel(lSumArray);
        }
        break;

    case 3 : //GAParameter::STOCHASTIC_REMAINDER :
        while (i-- > 0)
        {
            mMatingPool[i] = this->stochasticRemainder(lSumArray,i);
        }

        // Random shuffle of individual in mating pool to avoid same genomes to be contigous
        i=0;
        while
                ( i < this->mMatingSize )
        {
            j = (int)(lRandom->getDouble()*(this->mMatingSize - i)) +i;
            k = mMatingPool[i];
            mMatingPool[i] = mMatingPool[j];
            mMatingPool[j] = k;
            i++;
        }
        lAux = NULL;
        break;
    case 4: // simple selection
        //   delete this->mMatingPool;

        // this->mMatingPool=new int [this->mSelected.size()];
        //int j=this->mSelected.size();
        while (i-- > 0)
        {
            mMatingPool[i] = i;// this->mSelected[i];
        }



        break;

    default :	//----- TOURNAMENT
        if
                ( this->mGAParameter->minimize() )
        {
            while (i-- > 0)
            {
                mMatingPool[i] = this->reverseTournament(lNotesArray);
            }
        }
        else
        {
            while (i-- > 0)
            {
                mMatingPool[i] = this->tournament(lNotesArray);
            }
        }
        break;
    }

    delete[] lNotesArray;
    delete[] lShareArray;
    delete[] lSumArray;
    delete[] lResultArray;

}


void SPopulation::crossoverSimpleSelection()
{
    int lCpt;
    int lNbCrossover;
    int lIndexParent1;
    int lIndexParent2;


    Random* lRandom = NULL;
    lRandom->getInstance();

    Genome* lGenomeParent1 = NULL;
    Genome* lGenomeParent2 = NULL;

    Genome* lOffspringA = NULL;
    Genome* lOffspringB = NULL;


    int survivor = 0;

    lNbCrossover = (int) (this->mNbGenomes * this->mCrossoverRate);

    lCpt = 0;

    if ( this->mMatingSize % 2 == 1 )
    {
        survivor=mSelected[lRandom->getDouble()*mSelected.size()];
        mOffsprings[lCpt] = this->mGenomesArray[mMatingPool[survivor]];
        mChangesArray[lCpt] = false;
        mSurvived[mMatingPool[survivor]] = true;
        lCpt++;
    }


    if(this->mSelected.size()>=2){



        while(lCpt<lNbCrossover ){

            lIndexParent1=mSelected[lRandom->getDouble()*mSelected.size()];
            lIndexParent2=mSelected[lRandom->getDouble()*mSelected.size()];
            lGenomeParent1 = mGenomesArray[mMatingPool[lIndexParent1]];
            lGenomeParent2 = mGenomesArray[mMatingPool[lIndexParent2]];
            while (lGenomeParent1==lGenomeParent2){
                lIndexParent2=mSelected[lRandom->getDouble()*mSelected.size()];
                lGenomeParent2 = mGenomesArray[mMatingPool[lIndexParent2]];
            }

            lOffspringA = lGenomeParent1->clone();
            lOffspringB = lGenomeParent2->clone();

            lGenomeParent1->cross(lGenomeParent2, lOffspringA, lOffspringB);


            mOffsprings[lCpt] = lOffspringA;
            mChangesArray[lCpt] = true;
            lCpt++;

            mOffsprings[lCpt] = lOffspringB;
            mChangesArray[lCpt] = true;
            lCpt++;
        }


    }
    while (lCpt<this->mMatingSize){
        lIndexParent1=mSelected[lRandom->getDouble()*mSelected.size()];
        lGenomeParent1 = mGenomesArray[mMatingPool[lIndexParent1]];

        if ( mSurvived[mMatingPool[lIndexParent1]] )
        {
            mOffsprings[lCpt] = lGenomeParent1->copy();
            //mOffsprings[lCpt]->mutate();
        }
        else
        {
            mOffsprings[lCpt] = lGenomeParent1;

            //  mOffsprings[lCpt]->mutate();
            mSurvived[mMatingPool[lIndexParent1]] = true;
        }
        for (int i=0;i<mOffsprings[lCpt]->size();i++){
            for (unsigned int j=0;j<mOffsprings[lCpt]->getChromosome(i)->size();j++){
                if ( lRandom->getDouble()<this->mMutationRate){
                    ((ComplexGene *)mOffsprings[lCpt]->getChromosome(i)->getGene(j))->getGene(1)->mutate();
                    mChangesArray[lCpt] = true;
                }
            }
        }

        lCpt++;


    }



}


/******************************************************************************
 * mutation des enfants
 ******************************************************************************/
void SPopulation::mutation()
{
    Random* lRandom = NULL;
    lRandom->getInstance();
    for (int i=0 ; i<this->mMatingSize ; i++)
    {
        if ( lRandom->getDouble() < this->mMutationRate )
        {
            if (!mChangesArray[i]){

                mOffsprings[i]->mutate();
                mChangesArray[i] = true;
            }
        }
    }
}


/******************************************************************************
 * passage à la nouvelle génération
 ******************************************************************************/
void SPopulation::replacement()
{
    Genome* lGenome;

    if ( this->mNbGenomes == this->mMatingSize ) // Classical case
    {
        for (int i = this->mNbGenomes-1 ; i>=0 ; i--)
        {

            lGenome = mOffsprings[i];

            /*----- Passage de pp à this -----*/
            if ( ! mSurvived[i]	)
            {
                delete this->mGenomesArray[i];
            }

            this->mGenomesArray[i] = lGenome;

            mOffsprings[i] = NULL;
        }
    }

    else
    {
        double* lNoteArray = new double[this->mNbGenomes];
        int* lSortedPop = new int[this->mNbGenomes];

        for (int i =0; i<this->mNbGenomes; i++)
        {
            lSortedPop[i] = i;
            lNoteArray[i] = this->mGenomesArray[i]->getNote();
        }

        sortIndividuals(lSortedPop, 0, mNbGenomes-1, lNoteArray);

        for (int j=0; j<this->mMatingSize; j++)
        {
            lGenome = mOffsprings[j];

            /*----- Passage de pp à this -----*/
            if
                    ( ! mSurvived[lSortedPop[j]]	)
            {
                delete this->mGenomesArray[lSortedPop[j]];
            }

            this->mGenomesArray[lSortedPop[j]] = lGenome;

            mOffsprings[j] = NULL;
        }

        delete[] lSortedPop;
        delete[] lNoteArray;
    }
}



/*****************************************************************************
 * Passage à‡ la géénéération suivante.
 *****************************************************************************/
void SPopulation::generation()
{
    time_t lTime;

    time(&lTime);
    this->mDurations[0] = lTime;

    /*----- Variables locales -----*/

    Genome* lBestIndividual = NULL;

    /*----- Mise à‡ jour des paramétres de l'algorithme -----*/

    this->updateGAParameter();

    this->initGeneration();

    if ( this->mElitism )
    {
        lBestIndividual = this->mGenomesArray[this->mMaxPosition]->copy();
        mChangesArray[this->mMaxPosition] = false;
        mSurvived[this->mMaxPosition] = false;
    }

    /*----- Méthode de sélection -----*/

    time(&lTime);
    this->mDurations[1] = lTime;

    this->selection();


    /*----- Croisement : Passage de la population p1 à p2 -----*/

    time(&lTime);
    this->mDurations[2] = lTime;

    this->crossoverSimpleSelection();
    //this->crossover();


    /*----- Mutation sur la population p2 -----*/

    time(&lTime);
    this->mDurations[3] = lTime;

    this->mutation();


    /*----- Mise à jour de la population et notation -----*/
    time(&lTime);
    this->mDurations[4] = lTime;

    this->replacement();


    /*----- Copie du meilleur dans la génération suivante -----*/
    if
            ( this->mElitism )
    {
        delete mGenomesArray[this->mMaxPosition];
        mGenomesArray[this->mMaxPosition] = lBestIndividual;
        mChangesArray[this->mMaxPosition] = false;
    }


    /*----- Calcul de la fitness -----*/
    // sortis de la fonction pour aller dans le main. caroline
    //     this->setGenomesSymbolsArray();
    //     this->fitnessPopulation(); //en attente reponse client TODO

    /*----- Calculs statistiques et durée de la génération -----*/
    time(&lTime);
    this->mDurations[5] = lTime;

    this->statistics();

    if
            (	this->mRecordStats	)
    {
        this->mStatIndex = (this->mStatIndex+1) % this->mStatsLength;
        this->mMaxStats[this->mStatIndex] = this->mMaxNote;
        this->mAverageStats[this->mStatIndex] = this->mAverageNotes[0];
    }

    if
            ( this->mWriteToStatFile )
    {
        this->writeToStatFile();
    }

    lBestIndividual = NULL;

    delete[] mMatingPool;
    delete[] mChangesArray;
    delete[] mSurvived;

    time(&lTime);
    this->mDurations[6] = lTime;

} /*----- Fin de generation -----*/


