#include <QtGui>
#include <QtOpenGL>
#include <QKeyEvent>
#include "GL/gl.h"
#include <iostream>
#include <stack>
#include <Random.h>
#include "glwidget.h"
#include "symbol.h"
#include "turtle.h"
#include "icosphere.h"
#include <Convol/include/functors/Kernels/CompactPolynomial/CompactPolynomial6T.h>
#include <Convol/include/ScalarFields/BlobtreeNode/NodeOperators/NodeOperatorNCorrectedConvolutionT.h>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include <OpenMesh/Core/IO/exporter/ExporterT.hh>
#include <OpenMesh/Core/IO/writer/STLWriter.hh>
#include <OpenMesh/Core/IO/writer/STLWriter.cc>
#include <OpenMesh/Core/IO/writer/PLYWriter.hh>
#include <OpenMesh/Core/IO/writer/PLYWriter.cc>
#include <OpenMesh/Tools/Utils/getopt.h>
#include <OpenMesh/Core/Utils/Property.hh>
#define CHKWOPT( Option ) \
  std::cout << "  write " << #Option \
            << (wopt.check(OpenMesh::IO::Options:: Option)?": yes\n":": no\n")

#define MESHOPT( msg, tf ) \
  std::cout << "  " << msg << ": " << ((tf)?"yes\n":"no\n")

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

using namespace std;

//! [0]
GLWidget::GLWidget(QWidget *parent, std::vector<Symbol *> symv, int ID)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    this->ID=ID;
    symbolv = symv;
    mSelected=false;
    xRot = 0;
    yRot = 0;
    zRot = 0;
    scale=1.7f;
    colors.push_back(QColor(0,0,0));
    colors.push_back(QColor(1,1,1));

    blobt = new Convol::BlobtreeRootT<AppTraits>();
    skel = new Convol::SkeletonT<AppTraits>(blobt);
    kernel = new Convol::CompactPolynomial6T<AppTraits>(2.0) ;

    implicitsurf = new Convol::ImplicitSurfaceT<AppTraits>(blobt,1.0) ;

    prepareImplicitSurface();
}
//! [0]

//! [1]
GLWidget::~GLWidget()
{

    symbolv.clear();
    delete kernel;
    delete implicitsurf;
    //delete skel;
    delete blobt;

}
//! [1]

//! [2]
QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}
//! [2]


QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}
//! [4]

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

//! [5]
void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}
//! [5]

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

//! [6]
void GLWidget::initializeGL()
{



    //mylsys->setColor(qtGreen.dark());
    GLfloat sun_direction[] = { 0.0, 2.0, -1.0, 1.0 };
    GLfloat sun_intensity[] = { .5f,.5f,1.f,1.f};
    GLfloat ambient_intensity[] = { 0.3, 0.3, 0.3, 1.0 };
    //glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

}
//! [6]

//! [7]
void GLWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0, 0.0, -20.0);
    glRotatef(xRot / 8.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 8.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 8.0, 0.0, 0.0, 1.0);
    glScalef(scale,scale,scale);
    this->draw();
}
//! [7]

//! [8]
void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float d=10;
#ifdef QT_OPENGL_ES_1
    glOrthof(-d*0.5, +d*0.5, -d*0.5, +d*0.5, d-4.0,d+15);
#else
    glOrtho(-d*0.5, +d*0.5, -d*0.5, +d*0.5, d-4.0,d+15);
#endif
    glMatrixMode(GL_MODELVIEW);


}
//! [8]

//! [9]
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();

}
//! [9]

//! [10]
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();

}
//! [10]



void GLWidget::setSymbolVector(vector<Symbol*> symv){
    this->symbolv=symv;
    prepareImplicitSurface();
    updateGL();
}


void GLWidget::setColors(std::vector<QColor> colorv){
    this->colors = colorv;
}

void GLWidget::changeSelection(){
    if (!isSelected()){
        mSelected=true;

    }else{
        mSelected=false;

    }
}

bool GLWidget::isSelected(){
    return mSelected;
}

void GLWidget::wheelEvent(QWheelEvent * event){

    event->delta()>0?scale+=scale*0.1f: scale-=scale*0.1f;
    updateGL();

}

void GLWidget::draw(){
    if (symbolv.size()>0){


        for (AppTraits::TriMesh::FaceIter f_it = trim.faces_begin(); f_it != trim.faces_end(); ++f_it)
        {
            glBegin(GL_POLYGON);      // Get the face-vertex circulator of face _fh
            for (AppTraits::TriMesh::FaceVertexIter fv_it = trim.fv_iter(f_it); fv_it; ++fv_it)
            {
                //qglColor();
                glColor3dv(&(trim.color(fv_it)[0]));
                glNormal3dv(&(trim.normal(fv_it)[0]));
                glVertex3dv(&(trim.point(fv_it)[0]));
            }
            glEnd();
        }

    }
}


void GLWidget::prepareImplicitSurface(){

    skel->clear();
    blobt->Clear();
    //trim=0;
    /*if (trim!=0)
        delete trim;*/
    trim = AppTraits::TriMesh();

    Convol::NodeOperatorNCorrectedConvolutionT<Convol::CompactPolynomial6T<AppTraits> >* op_node =
            new Convol::NodeOperatorNCorrectedConvolutionT<Convol::CompactPolynomial6T<AppTraits> >();
    skel->nonconst_blobtree_root().AddChildToNAryNode(&(skel->nonconst_blobtree_root()), op_node);


    this->turtle=Turtle();
    QVector3D p=QVector3D();

    AppTraits::Point pp(0.0, 0.0, 0.0);
    SkelVHandle* last_vh = new SkelVHandle(pp, 1.0, op_node, skel);
    SkelVHandle* cur_vh = last_vh;

    std::stack<SkelVHandle*> vh_stack;

    float plop = 0.;

    foreach (Symbol *s, this->symbolv)
    {

        this->turtle.interpretSymbol(s);


            p = this->turtle.getTransformMatrix().map(QVector3D());
            pp = AppTraits::Point(p.x(), p.y(), p.z());

            if (s->getName() == 'f')
            {
                int r=rand()%2;
                QColor color(colors[r]);

                /*if (plop < 255.)
                    plop += 25.;*/
                cur_vh = new SkelVHandle(pp, 1.0, op_node, skel);

                /*double weight=1.;


                Random * r=NULL;
                r->getInstance();
                weight=1-r->getDouble()*0.5;

                cur_vh->set_weight(weight);
                delete r;*/
                SkelHSegment* seg = SkelHSegment::Create(*skel, last_vh, cur_vh, *static_cast<Convol::CompactPolynomial6T<AppTraits>* >(kernel));
                last_vh = cur_vh;

                seg->set_density_p1(1.0);
                seg->set_density_p2(1.0);
                seg->SetVectorProperty(AppTraits::E_Direction, seg->unit_dir());

                // Set color constraints
                std::vector< Convol::PropConstraint1DT<AppTraits::Scalar,AppTraits::Scalar> > color_red_constraints;
                std::vector< Convol::PropConstraint1DT<AppTraits::Scalar,AppTraits::Scalar> > color_green_constraints;
                std::vector< Convol::PropConstraint1DT<AppTraits::Scalar,AppTraits::Scalar> > color_blue_constraints;

                color_red_constraints.push_back(Convol::PropConstraint1DT<AppTraits::Scalar,AppTraits::Scalar>(0,/*color.red()/255.*/1.-color.red() / 255.));
                color_green_constraints.push_back(Convol::PropConstraint1DT<AppTraits::Scalar,AppTraits::Scalar>(0,/*color.green()/255.*/1.-color.green() / 255.));
                color_blue_constraints.push_back(Convol::PropConstraint1DT<AppTraits::Scalar,AppTraits::Scalar>(0,1.-color.blue()/255.));
                seg->SetScalarPropertyConstraints(AppTraits::E_Red,color_red_constraints);
                seg->SetScalarPropertyConstraints(AppTraits::E_Green,color_green_constraints);
                seg->SetScalarPropertyConstraints(AppTraits::E_Blue,color_blue_constraints);
            }
            else if (s->getName() == ']') // pop
            {
                last_vh = vh_stack.top();
                vh_stack.pop();
            }
            else if (s->getName() == '[') // push
            {
                vh_stack.push(last_vh);
                // new base point
            }


    }

    blobt->PrepareForEval(0.001, 0.1);
    Convol::tools::BasicMarchingCube<AppTraits>::AxisBoundingBox abb = blobt->GetAxisBoundingBox(0.9);

    bool b=false;
    foreach (Symbol * s,symbolv){
        if (s->getName()=='f'){
            b=true;
            break;
        }
    }

    if (symbolv.size()>0){  //Convol::tools::BasicMarchingCube<AppTraits> marcher(implicitsurf, 0.4, abb, 0.0, &trim);
        if (b){
            Convol::tools::BasicMarchingCube<AppTraits> marcher(implicitsurf, 0.2, abb, 0.0, &trim);
            marcher.ComputeGrid();
            marcher.March(true);
        }else{
            cout <<"ici"<<endl;
        }


    }





}

void GLWidget::exportMesh(){


  std::string filename=QString("%1/shape_%2.%3")
            .arg(QCoreApplication::applicationDirPath())
            .arg(QString::number(this->ID))
            .arg("ply").toStdString();
    cout << filename<<endl;

   OpenMesh::IO::Options wopt;

   wopt += OpenMesh::IO::Options::VertexNormal;
   wopt += OpenMesh::IO::Options::VertexColor;


   std::cout << "  write VertexNormal"
               << (wopt.check(OpenMesh::IO::Options::VertexNormal) ? ": yes\n":": no\n");
     CHKWOPT( VertexColor    );
     CHKWOPT( VertexTexCoord );
     CHKWOPT( FaceNormal     );
     CHKWOPT( FaceColor      );

     // -------------------- show mesh capabilities

     std::cout << "Mesh supports\n";
     MESHOPT("vertex normals", trim.has_vertex_normals());
     MESHOPT("vertex colors", trim.has_vertex_colors());
     MESHOPT("texcoords", trim.has_vertex_texcoords2D());
     MESHOPT("face normals", trim.has_face_normals());
     MESHOPT("face colors", trim.has_face_colors());
cout <<endl;

   if (!OpenMesh::IO::write_mesh(trim, filename,wopt)){
       std::cerr << "write error\n";
         exit(1);
   }

}


void GLWidget::clear(){

    mSelected=false;

    symbolv.clear();
    skel->clear();
    blobt->Clear();
    updateGL();
}
