#ifndef PLSYS_H
#define PLSYS_H
#include <vector>
#include <QObject>
#include <QVector3D>
#include <QHash>

#include "rule.h"
#include "turtlesymbol.h"
#include "turtle.h"

class PLSys : public QObject
{
public:
    std::vector<Symbol*> axiom;
    QHash<Symbol*,char> alphabet;
    std::vector<Rule> rules;
    int nbIterations;
    //Turtle t ;

    std::vector<Symbol*> pllist;

    std::vector<Symbol*> mapTS;

public:
    PLSys();
    ~PLSys();

    explicit PLSys(const PLSys &me);
    PLSys(std::vector<Symbol*> axiom, std::vector<Rule> rules );

    PLSys(QObject *parent,std::vector<Symbol*> alphabet, std::vector<Symbol*> axiom, std::vector<Rule> rules, int iter=0);

    std::vector<Symbol*> getPllist();
    Symbol *getPllist(int i);
    void addToAlphabet(Symbol *s);
    int setAlphabet(std::vector<Symbol *> alphabet);
    bool inAlphabet(Symbol *s);
    void setAxiom(std::vector<Symbol*> axiom);
    void setRules(std::vector<Rule> rules);
    bool initRules(std::vector<Rule> rules);
    bool initAxiom(std::vector<Symbol *> axiom);
    void iterate();
    void setIteration(int iter);
    void addRule(Rule r);
    void afficher(std::vector<Symbol*> symlist,int iter=0);
    void setPLList(std::vector<Symbol *> symbollist);
    bool feedmap(Symbol * s);

    //void draw();
};

#endif // PLSYS_H
