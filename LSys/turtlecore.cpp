#include "turtlecore.h"

TurtleCore::TurtleCore()
{
    transformMatrix.setToIdentity();
}

/* Change turtle's position/orientation/scale
 * @param matrix Transformation matrix
 */
void TurtleCore::transform(QMatrix4x4 matrix){
    this->transformMatrix.operator *=(matrix);
}

/* @return Turtle's transformation matrix */
QMatrix4x4 TurtleCore::getTransformMatrix() {
    return this->transformMatrix;
}

/* @param transformMatrix Turtle's transformation matrix*/
void TurtleCore::setTransformMatrix(QMatrix4x4 transformMatrix) {
    this->transformMatrix = transformMatrix;
}
