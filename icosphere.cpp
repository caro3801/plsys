#include "icosphere.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>

#define rat_octahedron_solid_base   1
#define rat_icosahedron_solid_base  2

#define _A 0.525731112119133606f
#define _B 0.850650808352039932f

#ifndef MATH_PI
#   ifndef M_PI
#       define MATH_PI 3.14159f
#   else
#       define MATH_PI M_PI
#   endif
#endif

static unsigned int octa_indices[8][3]=
{
    {0,1,2},{0,2,3},
    {0,3,4},{0,4,1},
    {5,2,1},{5,3,2},
    {5,4,3},{5,1,4}
};

static float octa_verts[6][3]=
{
    {0,0,-1},{1,0,0},
    {0,-1,0},{-1,0,0},
    {0,1,0},{0,0,1}
};

static unsigned int icosa_indices[20][3]=
{
    {0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
    {8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
    {7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
    {6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
};

static float icosa_verts[12][3]=
{
    {_A,0.0,-_B},{-_A,0.0,-_B},{_A,0.0,_B},{-_A,0.0,_B},
    {0.0,-_B,-_A},{0.0,-_B,_A},{0.0,_B,-_A},{0.0,_B,_A},
    {-_B,-_A,0.0},{_B,-_A,0.0},{-_B,_A,0.0},{_B,_A,0.0}
};

inline static void normalize_vert(float *a)
{
    float d=sqrtf(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    a[0]/=d; a[1]/=d; a[2]/=d;
}

void Icosphere::draw_recursive_tri(float *a,float *b,float *c,unsigned int div,float r)
{
    if (div==0)
    {
        glNormal3fv(a);
        glVertex3f(a[0]*r,a[1]*r,a[2]*r);

        glNormal3fv(b);
        glVertex3f(b[0]*r,b[1]*r,b[2]*r);

        glNormal3fv(c);
        glVertex3f(c[0]*r,c[1]*r,c[2]*r);
    }
    else
    {
        register unsigned int i;
        float ab[3],ac[3],bc[3];
        for (i=0; i<3; i++)
        {
            ab[i]=(a[i]+b[i])/2.0f;
            ac[i]=(a[i]+c[i])/2.0f;
            bc[i]=(b[i]+c[i])/2.0f;
        }

        normalize_vert(ab);
        normalize_vert(ac);
        normalize_vert(bc);

        draw_recursive_tri(a,ab,ac,div-1,r);
        draw_recursive_tri(b,bc,ab,div-1,r);
        draw_recursive_tri(c,ac,bc,div-1,r);
        draw_recursive_tri(ab,bc,ac,div-1,r);
    }
}

void Icosphere::draw(GLfloat x, GLfloat y, GLfloat z)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x,y,z);

    register unsigned int i;
    switch (solid_base)
    {
    case rat_octahedron_solid_base:
        glBegin(GL_TRIANGLES);
        for (i=0; i<8; i++)
            draw_recursive_tri
            (
                octa_verts[octa_indices[i][0]],
                octa_verts[octa_indices[i][1]],
                octa_verts[octa_indices[i][2]],
                detail,radius
            );
        glEnd();
        break;
    case rat_icosahedron_solid_base:
        glBegin(GL_TRIANGLES);
        for (i=0; i<20; i++)
            draw_recursive_tri
            (
                icosa_verts[icosa_indices[i][0]],
                icosa_verts[icosa_indices[i][1]],
                icosa_verts[icosa_indices[i][2]],
                detail,radius
            );
        glEnd();
        break;
    default:
        break;
    };
        glPopMatrix();
}
Icosphere::Icosphere(unsigned int detail,float radius,int solid_base)
{
    this->detail=detail;
    this->radius=radius;
    this->solid_base=solid_base;
}


