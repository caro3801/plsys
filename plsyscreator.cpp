#include "plsyscreator.h"
#include "functions.h"
#include "ui_plsyscreator.h"

PLSysCreator::PLSysCreator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PLSysCreator)
{
    ui->setupUi(this);
    mPLsys= new PLSys();

    modele = new QStringListModel( this);
    //modele->insertRow(0);
    modele->setStringList(alphabet);
    //ui->listViewAlphabet->setModel(modele);
    //ui->listViewAlphabetFrom->setModel(modele);
    ui->listViewSymbols->setModel(modele);
    connect(ui->pushButtonAddNewSymbol,SIGNAL(clicked()),this,SLOT(addSymbol()));

}

PLSysCreator::~PLSysCreator()
{
    delete ui;
}



void PLSysCreator::addSymbol(){
    QString s=ui->lineEditSymbolName->text();
    char c=s.toStdString().c_str()[0];
    double val=ui->doubleSpinBox->value();

    if(c!=' '){
        if (c=='+' ||c=='-' ||c=='/' ||c=='\\' || c=='f' ||c=='<' ||c=='>' ){
            mPLsys->addToAlphabet(new TurtleSymbol(c,parametres(1,(param){'x',val})));
        }else{
            mPLsys->addToAlphabet(new Letter(c,parametres(1,(param){'x',val})));
        }
    }
    updateAlphabetList();

}
void PLSysCreator::editSymbol(){ }
void PLSysCreator::addRule(){ }
void PLSysCreator::editRule(){ }
void PLSysCreator::addToAxiom(){ }
void PLSysCreator::removeFromAxiom(){ }
void PLSysCreator::setAxiom(){ }

void PLSysCreator::updateAlphabetList(){
    alphabet.clear();
    QList<char> qc=mPLsys->alphabet.values();
    QListIterator<char> listIter( qc );
    while( listIter.hasNext() ){
        char c=listIter.next();
        alphabet.append(QString(c));
    }

    modele->setStringList(alphabet);
}
