#ifndef TURTLECORE_H
#define TURTLECORE_H
#include <QMatrix4x4>
class TurtleCore
{
private:
    /* Turtle's transformation matrix */
    QMatrix4x4 transformMatrix;
public:
    TurtleCore();
    void transform(QMatrix4x4 matrix);
    QMatrix4x4 getTransformMatrix();
    void setTransformMatrix(QMatrix4x4 transformMatrix);

};

#endif // TURTLECORE_H
