#include "symbol.h"
#include "QString"
#include <iostream>
using namespace std;
struct param;
Symbol::Symbol()
{
}
Symbol::~Symbol()
{
    this->params.clear();
}
Symbol::Symbol(char name, vector<param> p)
{
    try{

        int s=p.size();
        if (s==0 || s>2 ){
            throw string( "warning argument: argument problem" );

        }else if (s==2 && name != 'f'){
            throw string( "warning argument: argument problem" );

        }else {

            this->name=name;
            this->params=p;
            this->nbparams = p.size();
        }
    }
    catch ( const std::string & Msg )
    {
        std::clog << Msg<<this->getName();
    }
}
Symbol::Symbol(char name, double value){
    this->name=name;
    this->params.push_back((param){'x',value});
    this->nbparams=1;
}

char Symbol::getName(){
    return this->name;
}

int Symbol::getNbParams(){
    return this->nbparams;
}


double Symbol::get(char pnam){
    try{
        if (!this->params.empty()){
            for(int i=0;i<this->nbparams;i++){
                if (this->params[i].pname==pnam)
                {
                    return this->params[i].value;
                }
            }
            throw "L'argument ne peut être atteint par char";


        }else{
            throw "pas de parametre a atteindre";
        }




    }
    catch (const char * e){
        cerr << "ERREUR : " << e << endl;
        //exit(0);
    }

}

double Symbol::get(int pnum=0){

    try{
        if (!this->params.empty()){
            if(pnum<this->nbparams && pnum>=0)
            {
                return this->params[pnum].value;
            }
            else {
                throw "L'argument demandé n'existe pas";
            }
        }else{

            throw "il n'y a pas d'arg";
        }

    }
    catch (const char * e){

        cerr << "ERREUR : " << e << endl;
        //exit(0);
    }
}
void Symbol::set(int pnum, double val){

    try{
        if (!this->params.empty()){
            if(pnum<this->nbparams && pnum>=0)
            {
                this->params[pnum].value=val;
            }
            else {
                throw "L'argument demandé n'existe pas";
            }
        }else{

            throw "ce symbole n'accepte pas d'argument";
        }

    }
    catch (const char * e){

        cerr << "ERREUR : " << e << endl;
        //exit(0);
    }
}

void Symbol::set(char pnam, double val){

    try{
        bool changed=false;
        if (!this->params.empty()){
            for(int i=0;i<this->nbparams && !changed;i++){
                if (this->params[i].pname==pnam)
                {
                    this->params[i].value=val;
                    changed=true;
                }
            }
            if (!changed){
                throw "L'argument ne peut être atteint par char";
            }



        }else{
            throw "ce symbole n'accepte pas d'argument";
        }




    }
    catch (const char * e){
        cerr << "ERREUR : " << e << endl;
        //exit(0);
    }

}

void Symbol::set(double val){
    try{
        if (!this->params.empty()){

            this->params[0].value=val;
        }else{

            throw "ce symbole n'accepte pas d'argument";
        }

    }
    catch (const char * e){

        cerr << "ERREUR : " << e << endl;
        //exit(0);
    }
}

void Symbol::setNbParams(int nbparams){
    this->nbparams=nbparams;
}

void Symbol::afficher() {
    cout << this->toString() ;
}

string Symbol::toString() {
    QString s= QString(this->getName());

    if (this->params.size()!=0){
        s.append("(");

        for (int i=0;i<this->nbparams;i++) {
            s.append(QString::number(this->get(i)));
            if (i!=this->nbparams-1){s.append(",");}
        }
        s.append(")");
    }
    s.append(" ");

    return s.toStdString();
}


