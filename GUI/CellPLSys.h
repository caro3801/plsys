#ifndef CELLPLSYS_H
#define CELLPLSYS_H
#include "glwidget.h"
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
using namespace std;

class CellPLSys
{

public:

    CellPLSys(int id,vector<Symbol*> pSymbolV, QWidget *parent=0);
    ~CellPLSys();
    GLWidget * getGLWidget();
    int getID();
    QCheckBox *createSelectCheckBox();

    void setColorV(std::vector<QColor> colorv);
    void setSymbolV(std::vector<Symbol*> symv);
    bool isSelected();
    QPushButton * getExportButton();

    void clear();

    QVBoxLayout *v;
    void uncheck();
signals:
    void stateChanged(int state);

    void clicked();

private:
    int ID;
    QHBoxLayout * h;
    GLWidget *mGLWidget;
    QCheckBox * mSelected;
    QPushButton * mExport;
    std::vector<Symbol*> mSymbolV;

};

#endif // CELLPLSYS_H
