#include "symbolforward.h"
static double modulo(double x, double y)
{
    /*x modulo y*/
    x-=y*std::abs(int(x/y));
    if (x>=0.) return (x);
    else return (x+y);
}

SymbolForward::SymbolForward(char name, double value):Symbol( name,value){
    max=1.2;
    min=0.1;
    mod=0.3;
    if (value>max || value < min){
        value=modulo(value,mod);
        if (value <min)
            value+=min;
        this->set(value);
    }
}
