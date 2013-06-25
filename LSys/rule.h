#ifndef RULE_H
#define RULE_H
#include "symbol.h"
#include <QHash>
#include <QList>

class Rule
{
public:
    Symbol* sym;
    bool (*condition)(Symbol*);
    std::vector<Symbol*>  (*sres)(Symbol*);

public:
    Symbol *getSymbol();
    Rule(Symbol* sym, bool (*cond)(Symbol*), std::vector<Symbol*> (*rep)(Symbol*));
    std::vector<Symbol *> apply(Symbol *s);
    bool isApplicable(Symbol *s);
};

#endif // RULE_H
