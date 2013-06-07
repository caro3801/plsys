#ifndef TURTLESYMBOL_H
#define TURTLESYMBOL_H
#include "symbol.h"

class TurtleSymbol : public Symbol
{

public:
    TurtleSymbol();
    ~TurtleSymbol();
    TurtleSymbol(char name, std::vector<param> p);
    void afficher();
    void set(double val);
    std::string toString();
};

#endif // TURTLESYMBOL_H
