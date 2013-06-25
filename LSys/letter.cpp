#include "letter.h"
#include <iostream>
using namespace std;

Letter::Letter()
{
}

Letter::~Letter(){
    params.clear();
}

Letter::Letter(char name, vector<param> p) :  Symbol(name,p)
{

    try
    {
        (p.size()>0)?setNbParams(p.size()):throw string( "Error argument: Pas assez de paramètres pour " );

    }
    catch ( const std::string & Msg )
    {
        std::cerr << Msg<<this->getName();
    }
}

void Letter::afficher(){
    cout << this->getName();
    cout << "(";

    for (int i=0;i<this->nbparams;i++) {
        cout <<this->params[i].pname;
        cout << ":";
        cout <<this->params[i].value;
        if (i!=this->nbparams-1){cout <<",";}
    }

    cout << ") ";
}

string Letter::toString(){
    string s= ""+this->getName();
    s+= "(";

    for (int i=0;i<this->nbparams;i++) {
        s+=this->params[i].pname;
        s+= ":";
        s+= this->params[i].value;
        if (i!=this->nbparams-1){s+=",";}
    }

    s+= ") ";
    return s;
}

