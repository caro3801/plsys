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
    double min;
    double max;
    double mod;
public:
    virtual double getMin();
    virtual double getMax();
    virtual double getMod();
    Symbol();
    Symbol(char name, std::vector<param> p);
    ~Symbol();
    virtual void afficher();
    virtual std::string toString();


    Symbol(char name, double value);
    Symbol(char name);
    virtual char getName();
    virtual  void set(int pnum,double val);
    virtual  void set(double val);
    virtual double get(char pnam);
    virtual double get(int pnum);
    virtual void set(char pnam, double val);
};


#endif // SYMBOL_H
