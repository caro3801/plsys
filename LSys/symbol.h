#ifndef SYMBOL_H
#define SYMBOL_H
#include <vector>
#include <string>
#include "param.h"
struct param;
class Symbol
{

protected:
    char name;
    int nbparams;
    std::vector<param> params;

public:
    Symbol();
    Symbol(char name, std::vector<param> p);
    Symbol(char name, double value);
    ~Symbol();
    void afficher();
    std::string toString();

    char getName();
    int getNbParams();
    double get(char pnam);
    double get(int pnum);
    void set(int pnum,double val);
    void set(char pnam, double val);
    void set(double val);
    void setNbParams(int);
};

#endif // SYMBOL_H
