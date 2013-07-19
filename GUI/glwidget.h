#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "config.h"
#include <Convol/include/skeleton/SkeletonT.h>
#include <Convol/include/ImplicitSurfaceT.h>
#include <Convol/include/skeleton/SkelVertexHandleCorrectorT.h>
#include <Convol/include/skeleton/SkelHomotheticSegmentT.h>
#include <Convol-Tools/include/Polygonizer/MarchingCube.h>

#include <QGLWidget>

class PLSys;
class Symbol;
#include "turtle.h"


struct myMeshTraits: public AppTraits
{
typedef OpenMesh::Vec4f Color;
VertexAttributes(
OpenMesh::Attributes::Normal |
OpenMesh::Attributes::Color);
FaceAttributes(
OpenMesh::Attributes::Normal |
OpenMesh::Attributes::Color);
};


//! [0]
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:

    typedef OpenMesh::TriMesh_ArrayKernelT<myMeshTraits> Mesh;
    typedef Convol::SkelVertexHandleCorrectorT<Convol::CompactPolynomial6T<AppTraits> > SkelVHandle;
    typedef Convol::SkelHomotheticSegmentT<AppTraits> SkelHSegment;

public:
    GLWidget(QWidget *parent = 0, std::vector<Symbol*> symv=std::vector<Symbol *>(), int ID=-1);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setSymbolVector(std::vector<Symbol*> symv);
    void setColors(std::vector<QColor> colorv);
    bool isSelected();
    void prepareImplicitSurface();
    void clear();
//! [2]
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event);
    void draw();
//! [2]

//! [3]
private:
    std::vector<Symbol*> symbolv;

    Turtle turtle;
    int ID;
    int xRot;
    int yRot;
    int zRot;
    float scale;
    bool mSelected;
    QPoint lastPos;
    std::vector<QColor> colors;
    Convol::SkeletonT<AppTraits> *skel;
    Convol::BlobtreeRootT<AppTraits> *blobt;
    Convol::FunctorT<AppTraits> *kernel;
    AppTraits::TriMesh trim;
    Convol::ImplicitSurfaceT<AppTraits> *implicitsurf;

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void changeSelection();
    void exportMesh();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);



};
//! [3]

#endif // GLWIDGET_H
