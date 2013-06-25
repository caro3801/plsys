#include "rule.h"
#include <iostream>
using namespace std;

Symbol* Rule::getSymbol(){
    return this->sym;
}

Rule::Rule(Symbol *sym, bool (*cond)(Symbol*), vector<Symbol*> (*rep)(Symbol*))
{
    this->sym = sym;
    this->condition = *cond;
    this->sres=*rep;
}
vector<Symbol*>  Rule::apply(Symbol *s) {
    vector<Symbol*> q;

    if ((this->condition)(s)) {
        q=(this->sres)(s);
    }
    return q;
}

bool Rule::isApplicable(Symbol *s){
    return ((this->sym==s) && (this->condition)(s));
}
