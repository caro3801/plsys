#ifndef ICOSPHERE_H
#define ICOSPHERE_H
#include <QtOpenGL>
class Icosphere
{
public:
    unsigned int detail;
    float radius;
    int solid_base;

public:
    Icosphere(unsigned int detail,float radius,int solid_base);
void draw_recursive_tri(float *a,float *b,float *c,unsigned int div,float r);
void draw(GLfloat x, GLfloat y, GLfloat z);
};

#endif // ICOSPHERE_H
