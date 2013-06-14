#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "param.h"
#include "turtlesymbol.h"
#include "plsys.h"
#include "iostream"
using namespace std;
struct param;

/**/
static Symbol pa= Symbol('[');

static Symbol pb= Symbol(']');

static Symbol ta=  Symbol('+',22.5);
static Symbol bb=  Symbol('-',22.5);
static vector<param> parametres(int nbp, ...){
    param p[nbp];
    va_list ap;
    va_start(ap, nbp);
    for( int i = 0 ; i < nbp ; i++){
        p[i]=va_arg(ap, param);
    }
    va_end(ap);

    vector<param> res(p,p+ (sizeof p/ sizeof p[0]));
    return res;
}

//mes conditions
static bool sup(Symbol *l) {
    return l->get('x') > 30;
}
static bool infeq(Symbol* l) {
    return l->get('x') <= 30;
}
static bool alltrue(Symbol* l) {
    return true;
}
//mes retours
static vector<Symbol*>  tree(Symbol *l) {
    vector<Symbol*>  q;

     Symbol *pus=new Symbol('[');

     Symbol *po=new Symbol(']');

     Symbol *t= new Symbol('+',22.5);
     Symbol *b= new Symbol('-',22.5);
     //q.push_back(alphabet.get('['));
     /*
     q.push_back(l);
     q.push_back(pus);
     q.push_back(t);
     q.push_back(l);
     q.push_back(po);
     q.push_back(l);
     q.push_back(pus);
     q.push_back(b);
     q.push_back(l);
     q.push_back(po);
     q.push_back(l);*/
     q.push_back(l);
     q.push_back(&pa  );
     q.push_back(&ta);
     q.push_back(l);
     q.push_back(&pb);
     q.push_back(l);
     q.push_back(&pa);
     q.push_back(&bb);
     q.push_back(l);
     q.push_back(&pb);
     q.push_back(l);
    return q;
}


static vector<Symbol*> reponse(Symbol *l) {

    vector<Symbol*>  q;
    l->set('x',l->get('x')+0.1);
    q.push_back(l);
    q.push_back( new Symbol('<',32.0));
    q.push_back( new Symbol('\\',32.0));
    q.push_back( new Symbol('f',3.));
    return q;
}


static vector<Symbol*>  reponse2(Symbol *l) {
    vector<Symbol*>  q;

     q.push_back( new Symbol('['));
     q.push_back( new Symbol('f',1));
     q.push_back( new Symbol(']'));
     q.push_back(l);
    return q;
}

static PLSys* initglsystem(int iteration=0,QObject *parent=0){



    Symbol *f= new Symbol('f',3);
    //Init with axiom
    vector<Symbol *> alphabet;
    alphabet.push_back(f);

    vector<Symbol *> axiom;
    axiom.push_back(f);
    //Rules
    vector<Rule> rules;
    Rule rule1 = Rule(f,alltrue, tree);
    //Rule rule2 = Rule(t,sup,reponse);
    //Rule rule3 = Rule(r,infeq,reponse2);
    rules.push_back(rule1);
    //rules.push_back(rule2);
    //rules.push_back(rule3);
   // axiom.push_back(a2);
    return new PLSys(parent,alphabet, axiom,rules,iteration);
}

/**/
#endif // FUNCTIONS_H
