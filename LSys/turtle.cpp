#include "turtle.h"
#include <math.h>
#include <QMatrix4x4>
#include <iostream>
using namespace std;

Turtle::Turtle()
{
    this->stackM=stack<QMatrix4x4>();
    this->depth=0;
    this->maximalDepth=0;
    this->depthStack=stack<int>();
}

void Turtle::forward(double distance){

    QMatrix4x4 temp = QMatrix4x4();

    temp.setToIdentity();

    temp.translate(0,distance,0);

    this->depth++;
    if(this->depth>this->maximalDepth)
        this->maximalDepth=this->depth;
    transform(temp);
}

void Turtle::turn(double teta){
    QMatrix4x4 temp = QMatrix4x4();
    temp.setToIdentity();
    temp.rotate(-teta,0,0,1); //selon Z
    transform(temp);

}

void Turtle::pitch(double teta){

    QMatrix4x4 temp = QMatrix4x4();
    temp.setToIdentity();
    temp.rotate(teta,0,1,0); //selon Y
    transform(temp);

}

void Turtle::roll(double teta){
    QMatrix4x4 temp = QMatrix4x4();
    temp.setToIdentity();
    temp.rotate(teta,1,0,0); //selon X
    transform(temp);
}

void Turtle::push(){
    this->stackM.push(QMatrix4x4(getTransformMatrix()));
    this->depthStack.push(this->depth);
}

void Turtle::pop(){

    setTransformMatrix(this->stackM.top());
    this->stackM.pop();

    this->depth = this->depthStack.top();
    this->depthStack.pop();
}

int Turtle::getDepth()
{
    return this->depth;
}

int Turtle::getMaximalDepth()
{
    return this->maximalDepth;
}

void Turtle::scale(float x, float y, float z) {
    QMatrix4x4 temp = QMatrix4x4();
    temp.setToIdentity();
    temp.scale(x,y,z);
    transform(temp);
}


void Turtle::interpretSymbol(Symbol *symbol){


    char c = symbol->getName();
    switch (c){
    case ('f'):
    {
        forward(symbol->get('x'));
        break;
    }
    case ('+'):
    {
        turn(symbol->get('x'));
        break;
    }
    case ('-'):
    {
        turn(-symbol->get('x'));
        break;
    }
    case ('['):
    {
        push();
        break;
    }
    case (']'):
    {
        pop();
        break;
    }
    case ('<'):
    {
        roll(symbol->get('x'));
        break;
    }
    case ('>'):
    {
        roll(-symbol->get('x'));
        break;
    }
    case ('/'):
    {
        pitch(symbol->get('x'));
        break;
    }
    case ('\\'):
    {
        pitch(-symbol->get('x'));
        break;
    }
    /*case ('S'):
    {
        float scaleMultiplier = symbol->get('x');
        float scaleMultiplierX = symbol->parameterExists(Symbol.SCALE_MULTIPLIER_X)?symbol.getParameter(Symbol.SCALE_MULTIPLIER_X):scaleMultiplier;
        float scaleMultiplierY = symbol.parameterExists(Symbol.SCALE_MULTIPLIER_Y)?symbol.getParameter(Symbol.SCALE_MULTIPLIER_Y):scaleMultiplier;
        float scaleMultiplierZ = symbol.parameterExists(Symbol.SCALE_MULTIPLIER_Z)?symbol.getParameter(Symbol.SCALE_MULTIPLIER_Z):scaleMultiplier;

        scale(scaleMultiplierX, scaleMultiplierY, scaleMultiplierZ);

        break;
    }*/

    default:
        break;
    }

}



