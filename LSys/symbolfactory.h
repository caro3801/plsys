#ifndef SYMBOLFACTORY_H
#define SYMBOLFACTORY_H
#include "symbol.h"
class SymbolFactory
{
public:
    SymbolFactory();

    Symbol* create(char type);
    Symbol* create(char type,double value);
};

#endif // SYMBOLFACTORY_H
