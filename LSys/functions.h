#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "param.h"
#include "turtlesymbol.h"
#include "plsys.h"
#include "iostream"
#include "symbolfactory.h"
using namespace std;
struct param;


static vector<Symbol *> alphabet;



//mes conditions
static bool sup(Symbol *l) {
    return l->get('x') > 2.;
}
static bool infeq(Symbol* l) {
    return l->get('x') <= 2.;
}
static bool alltrue(Symbol* l) {
    return true;
}
//mes retours
static vector<Symbol*>  tree(Symbol *l) {
    vector<Symbol*>  q;

    /* Symbol *pus=new Symbol('[');

     Symbol *po=new Symbol(']');

     Symbol *t= new Symbol('+',22.5);
     Symbol *b= new Symbol('-',22.5);*/
     //q.push_back(alphabet.get('['));

     q.push_back(l);
     q.push_back(alphabet.at(0));
     q.push_back(alphabet.at(5));
     q.push_back(l);
     q.push_back(alphabet.at(1));
     q.push_back(l);
     q.push_back(alphabet.at(0));
     q.push_back(alphabet.at(8));
     q.push_back(l);
     q.push_back(alphabet.at(1));
     q.push_back(l);
    return q;
}


static vector<Symbol*> reponse(Symbol *l) {

    vector<Symbol*>  q;
    l->set('x',l->get('x')+0.1);
    q.push_back(l);
    q.push_back(alphabet.at(8));
    q.push_back( alphabet.at(9));
    q.push_back( alphabet.at(4));
    return q;
}


static vector<Symbol*>  reponse2(Symbol *l) {
    vector<Symbol*>  q;

     q.push_back( alphabet.at(0));
     q.push_back( alphabet.at(3));
     q.push_back( alphabet.at(9));
     q.push_back( alphabet.at(3));
     q.push_back( alphabet.at(1));
     q.push_back(l);
    return q;
}

static PLSys* initglsystem(int iteration=0,QObject *parent=0){

    SymbolFactory *sf= new SymbolFactory();


    /*Symbol *f= sf->create('f',0.4);
    Symbol *f2= sf->create('f',0.6);
    Symbol *po= sf->create(']');
    Symbol *pu= sf->create('[');

    Symbol *tr= sf->create('<',32);
    Symbol *tl= sf->create('>',128);
    Symbol *rr= sf->create('+',57);
    Symbol *rl= sf->create('-',274);
    Symbol *pr= sf->create('/',65);
    Symbol *pl= sf->create('\\',345);*/
    //Init with axiom
    string name="myPLSys";
    alphabet.push_back(sf->create('[')); //0
    alphabet.push_back(sf->create(']')); //1
    alphabet.push_back(sf->create('/',65)); //2
    alphabet.push_back(sf->create('f',0.6)); //3
    alphabet.push_back(sf->create('f',0.6)); //4
    alphabet.push_back(sf->create('-',274)); //5
    alphabet.push_back(sf->create('>',128)); //6
    alphabet.push_back(sf->create('<',32)); //7
    alphabet.push_back(sf->create('+',57)); //8
    alphabet.push_back(sf->create('\\',345)); //9

    vector<Symbol *> axiom;
    axiom.push_back(alphabet.at(0));
    axiom.push_back(alphabet.at(0));
    axiom.push_back(alphabet.at(3));
    axiom.push_back(alphabet.at(1));
    axiom.push_back(alphabet.at(5));
    axiom.push_back(alphabet.at(0));
    axiom.push_back(alphabet.at(3));
    axiom.push_back(alphabet.at(3));
    axiom.push_back(alphabet.at(1));
    axiom.push_back(alphabet.at(6));
    axiom.push_back(alphabet.at(3));

    axiom.push_back(alphabet.at(7));
    axiom.push_back(alphabet.at(3));
    axiom.push_back(alphabet.at(1));
    //Rules
    vector<Rule> rules;
    Rule rule1 = Rule(alphabet.at(3),sup, reponse);
    Rule rule5 = Rule(alphabet.at(3),infeq, reponse2);
    Rule rule2 = Rule(alphabet.at(6),sup,reponse);
    Rule rule3 = Rule(alphabet.at(7),sup,reponse);
    Rule rule4 = Rule(alphabet.at(7),infeq,reponse2);
    rules.push_back(rule1);
    rules.push_back(rule2);
    rules.push_back(rule3);
    rules.push_back(rule4);
    rules.push_back(rule5);



    return new PLSys(parent,name,alphabet, axiom,rules,iteration);
}

/**/
#endif // FUNCTIONS_H
