#include <GL/gl.h>
#include <string>
#include <iostream>
#include "symbol.h"
#include "turtlesymbol.h"
#include "plsys.h"

#include <math.h>

using namespace std;


PLSys::PLSys()
{
    this->nbIterations=0;
}
PLSys::~PLSys()
{
    this->alphabet.clear();
    this->axiom.clear();
    this->rules.clear();
    this->pllist.clear();
    this->nbIterations=0;
    this->mapTS.clear();
}

PLSys::PLSys(const PLSys &me)
{
    this->alphabet=me.alphabet;
    this->pllist=me.pllist;
    this->axiom=me.axiom;
    this->rules=me.rules;
    this->nbIterations=me.nbIterations;
    this->mapTS=me.mapTS;
}
PLSys::PLSys(std::vector<Symbol*> axiom, vector<Rule> rules ){
    nbIterations=0;
    initAxiom(axiom);
    setRules(rules);
}
PLSys::PLSys(QObject *parent,vector<Symbol*> alphabet,vector<Symbol*> axiom, vector<Rule> rules , int iter)
    :QObject(parent)
{
    nbIterations=0;
    setAlphabet(alphabet);
    bool axOK,ruOK=true;
    axOK=initAxiom(axiom);
    ruOK=initRules(rules);
    if (axOK && ruOK){
        setAxiom(axiom);
        setPLList(axiom);
        setRules(rules);
        setIteration(iter);
    }
}

std::vector<Symbol *> PLSys::getPllist(){
    return this->pllist;
}

Symbol* PLSys::getPllist(int i){
    return this->pllist[i];
}
void PLSys::setAxiom(std::vector<Symbol *> axiom){
    this->axiom=axiom;
}
void PLSys::setRules(vector<Rule> rules){
    this->rules=rules;
}
bool PLSys::initRules(vector<Rule> rules){

    foreach(Rule r, rules){
        QList<Symbol*> ls= this->alphabet.uniqueKeys();
        foreach (Symbol *s,ls){
            foreach(Symbol* srule,r.apply(s)){
                if (!this->alphabet.contains(srule)){
                    this->addToAlphabet(srule);
                }
            }
        }
    }
    return true;
}

bool PLSys::initAxiom(vector<Symbol*> axiom){
    foreach(Symbol * s, axiom){
        if(!this->alphabet.contains(s)){
            cout << s->getName()<<"erreur :: l'axiom contient des symboles qui ne sont pas dans l'alphabet"<< endl;
            return false;
        }
    }
    return true;
}

void PLSys::iterate(){

    vector<Symbol*> tmp;
    bool applied;
    foreach(Symbol * sym,this->pllist){


        applied=false;
        foreach (Rule rule, this->rules) {

            if (rule.isApplicable(sym) ){

                vector<Symbol*> symbols_from_rule = rule.apply(sym);
                int nbsymbol_returned=(int)symbols_from_rule.size();

                for (int k=0 ; k < nbsymbol_returned ; k++) {
                    if (this->alphabet.key(symbols_from_rule[k]->getName())!=NULL){
                        tmp.push_back(symbols_from_rule[k]);
                    }else{
                        cout << symbols_from_rule[k]->getName()<<"ce symbole n'appartient pas a l'alphabet"<<endl;
                    }
                }
                applied=true;
            }
        }
        if (!applied){
            tmp.push_back(sym);
        }
    }
    nbIterations++;
    setPLList(tmp);
    tmp.clear();

}

void PLSys::setIteration(int iter=0){
    if (iter >this->nbIterations) {
        while(this->nbIterations!=iter){
            this->iterate();
        }
    }/*else if (iter>this->nbIterations){
            this->desiterate();

        }*/


}

void PLSys::addRule(Rule r){
    this->rules.push_back(r);
}


void PLSys::afficher(std::vector<Symbol *> symlist,int iter){
    cout << "PLSys size:" << symlist.size()<< " iter: "<<iter<<endl;
    for (unsigned int l = 0 ; l < symlist.size() ; l++){
        symlist[l]->afficher();
    }
    cout <<endl;
}


void PLSys::addToAlphabet(Symbol *s){
    this->alphabet.insert(s,s->getName());
}

int PLSys::setAlphabet(std::vector<Symbol *> alphabet){
    int add=0;

    foreach(Symbol *s, alphabet){
        if (!inAlphabet(s)){
            addToAlphabet(s);
            add++;
        }else{
            cout << "ce symbole est deja dans l'alphabet"<<endl;

        }
    }
    return add;
}

bool PLSys::inAlphabet(Symbol * s){
    return this->alphabet.contains(s);
}

void PLSys::setPLList(vector<Symbol*> symbollist){
    this->pllist=symbollist;

    this->mapTS=vector<Symbol*>();
    unsigned int i=0;
    while(i<this->pllist.size()){
        feedmap(this->pllist[i]);
        i++;
    }
}

bool PLSys::feedmap(Symbol *s){
    if(!(std::find(this->mapTS.begin(), this->mapTS.end(), s) != this->mapTS.end())) {
        this->mapTS.push_back(s);
        return true;
    }

    return false;
}


