#include "CellPLSys.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
using namespace std;


CellPLSys::CellPLSys(int id, vector<Symbol *> pSymbolV, QWidget *parent)
{
    this->ID=id;

    mGLWidget= new GLWidget(parent,pSymbolV,id);


    mSelected=createSelectCheckBox();
    mExport = new QPushButton("Export");
    mExport->setIcon(QIcon("save.png"));
    mExport->setIconSize(QSize(40,40));
    mExport->setMaximumWidth(60);
    /*   this->addWidget(mGLWidget);
    this->addWidget(mSelected);
    this->addWidget(mExport);
   */
    QObject::connect(mSelected, SIGNAL(stateChanged(int)),this->getGLWidget(), SLOT(changeSelection()));

    QObject::connect(mExport, SIGNAL(clicked()),this->getGLWidget(), SLOT(exportMesh()));
    v= new QVBoxLayout();
    h= new QHBoxLayout();
    h->addWidget(mSelected);
    h->addWidget(mExport);
    v->addWidget(mGLWidget);
    v->addLayout(h);

}


CellPLSys::~CellPLSys(){
    delete mSelected;
    delete mExport;
    mSymbolV.clear();
    delete mGLWidget;
    delete h;
    delete v;
}

GLWidget * CellPLSys::getGLWidget(){
    return this->mGLWidget;
}

int CellPLSys::getID(){
    return this->ID;
}

QCheckBox *CellPLSys::createSelectCheckBox()
{

    QCheckBox *cb = new QCheckBox(QString("Shape %1").arg(this->ID));
    cb->setChecked(false);
    return cb;
}
void CellPLSys::setSymbolV(vector<Symbol*> symv){
    this->mGLWidget->setSymbolVector(symv);
}


bool CellPLSys::isSelected(){
    return this->mSelected->isChecked();
}

  QPushButton * CellPLSys::getExportButton(){
      return this->mExport;
  }

void CellPLSys::clear(){
    mSelected->setChecked(false);
    mGLWidget->clear();
}

