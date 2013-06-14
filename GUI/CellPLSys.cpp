#include "CellPLSys.h"
using namespace std;

CellPLSys::CellPLSys(int id, vector<Symbol *> pSymbolV, QWidget *parent):QVBoxLayout(parent)
   {
    mGLWidget= new GLWidget(parent,pSymbolV);
    this->ID=id;
    mSelected=createSelectCheckBox();
    mExport = new QPushButton("Export");
    mExport->setIcon(QIcon(":/img/save.png"));
    mExport->setIconSize(QSize(40,40));
    this->addWidget(mGLWidget);
    this->addWidget(mSelected);
    this->addWidget(mExport);
    connect(mSelected, SIGNAL(stateChanged(int)),this->getGLWidget(), SLOT(changeSelection()));
    //connect(mExport, SIGNAL(clicked()),this->getGLWidget(), SLOT(exportMesh()));
}


CellPLSys::~CellPLSys(){
    delete mGLWidget;
    delete mSelected;
    delete mExport;
    mSymbolV.clear();
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

void CellPLSys::clear(){
    mSelected->setChecked(false);
    mGLWidget->clear();
}

