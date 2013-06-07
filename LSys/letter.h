#ifndef LETTER_H
#define LETTER_H
#include "symbol.h"

class Letter : public Symbol
{
public:
    Letter();
    ~Letter();
    Letter(char name, std::vector<param> p);
    void afficher();
    std::string toString();
};


#endif // LETTER_H
