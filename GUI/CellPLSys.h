#ifndef CELLPLSYS_H
#define CELLPLSYS_H
#include "glwidget.h"
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
using namespace std;

class CellPLSys : public QVBoxLayout
{

public:

    CellPLSys(int id,vector<Symbol*> pSymbolV, QWidget *parent=0);
    ~CellPLSys();
    GLWidget * getGLWidget();
    int getID();
    QCheckBox *createSelectCheckBox();

    void setSymbolV(std::vector<Symbol*> symv);
    bool isSelected();

    void clear();

signals:
    void stateChanged(int state);

private:
    int ID;
    GLWidget *mGLWidget;
    QCheckBox * mSelected;
    QPushButton * mExport;
    std::vector<Symbol*> mSymbolV;
};

#endif // CELLPLSYS_H
