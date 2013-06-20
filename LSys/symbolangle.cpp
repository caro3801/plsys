#include "symbolangle.h"
static double modulo(double x, double y)
{
    /*x modulo y*/
    x-=y*std::abs(int(x/y));
    if (x>=0.) return (x);
    else return (x+y);
}
SymbolAngle::SymbolAngle(char name, double value):Symbol( name,value){
    max=360;
    min=0;
    mod=45;
    if (value>max || value < min){
        value=modulo(value,mod);
        this->set(value);
    }
}
