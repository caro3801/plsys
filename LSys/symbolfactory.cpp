#include "symbolfactory.h"
#include "symbolforward.h"
#include "symbolstack.h"
#include "symbolangle.h"

SymbolFactory::SymbolFactory()
{
}

Symbol* SymbolFactory::create(char type) {
    if ( type == ']' || type == '[') return new SymbolStack(type);
    return NULL;
}
Symbol* SymbolFactory::create(char type,double value) {
    if ( type == 'f' ) return new SymbolForward(type,value);

    if ( type == ']' || type == '[') return new SymbolStack(type);
    if ( type == '<' || type == '>'|| type == '/' || type == '\\' || type == '+' || type == '-' ) return new SymbolAngle(type,value);

    return NULL;
}
