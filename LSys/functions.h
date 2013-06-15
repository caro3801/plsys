#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "param.h"
#include "turtlesymbol.h"
#include "plsys.h"
#include "iostream"
using namespace std;
struct param;



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
    return l->get('x') > 2;
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
     q.push_back(l);
    return q;
}


static vector<Symbol*> reponse(Symbol *l) {

    vector<Symbol*>  q;
    l->set('x',l->get('x')+0.1);
    q.push_back(l);
    q.push_back( new Symbol('+',32.0));
    q.push_back( new Symbol('\\',32.0));
    q.push_back( new Symbol('f',1.3));
    return q;
}


static vector<Symbol*>  reponse2(Symbol *l) {
    vector<Symbol*>  q;

     q.push_back( new Symbol('['));
     q.push_back( new Symbol('f',0.2));
     q.push_back( new Symbol(']'));
     q.push_back(l);
    return q;
}

static PLSys* initglsystem(int iteration=0,QObject *parent=0){



    Symbol *f= new Symbol('f',0.4);
    Symbol *f2= new Symbol('f',0.6);
    Symbol *po= new Symbol(']');
    Symbol *pu= new Symbol('[');

    Symbol *tr= new Symbol('<',32);
    Symbol *tl= new Symbol('>',128);
    Symbol *rr= new Symbol('+',57);
    Symbol *rl= new Symbol('-',274);
    Symbol *pr= new Symbol('/',65);
    Symbol *pl= new Symbol('\\',345);
    //Init with axiom
    vector<Symbol *> alphabet;
    alphabet.push_back(pu);
    alphabet.push_back(po);
    alphabet.push_back(pr);
    alphabet.push_back(f);
    alphabet.push_back(f2);
    alphabet.push_back(rl);
    alphabet.push_back(tl);
    alphabet.push_back(tr);

    vector<Symbol *> axiom;
    axiom.push_back(f);
    //Rules
    vector<Rule> rules;
    Rule rule1 = Rule(f,sup, reponse);
    Rule rule5 = Rule(f,infeq, reponse2);
    Rule rule2 = Rule(tr,sup,reponse);
    Rule rule3 = Rule(tl,sup,reponse);
    Rule rule4 = Rule(tl,infeq,reponse2);
    rules.push_back(rule1);
    rules.push_back(rule2);
    rules.push_back(rule3);
    rules.push_back(rule4);
    rules.push_back(rule5);


    axiom.push_back(pu);
    axiom.push_back(pu);
    axiom.push_back(pr);
    axiom.push_back(f);
    axiom.push_back(rl);
    axiom.push_back(f);
    axiom.push_back(po);
    axiom.push_back(rl);
    axiom.push_back(f);
    axiom.push_back(f);
    axiom.push_back(tl);
    axiom.push_back(f2);
    axiom.push_back(pu);

    axiom.push_back(tr);
    axiom.push_back(f2);

    axiom.push_back(po);

    axiom.push_back(f2);
    return new PLSys(parent,alphabet, axiom,rules,iteration);
}

/**/
#endif // FUNCTIONS_H
