#ifndef TURTLE_H
#define TURTLE_H
#include <stack>
#include "symbol.h"
#include "turtlecore.h"

struct Point3d{
    double x;
    double y;
    double z;
};

class Turtle : public TurtleCore
{
private :
    std::stack<QMatrix4x4> stackM;
    int depth;
    int maximalDepth;
    std::stack<int> depthStack;

public:
    Turtle();
    void forward(double distance);
    void turn(double teta);
    void pitch(double teta);
    void roll(double teta);
    void push();
    void pop();
    int getDepth();
    int getMaximalDepth();
    void scale(float x, float y, float z) ;
    void interpretSymbol(Symbol *symbol);

};

#endif // TURTLE_H
