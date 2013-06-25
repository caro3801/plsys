#ifndef PLSYSCREATOR_H
#define PLSYSCREATOR_H

#include "plsys.h"
#include <QStringListModel>
#include <QDialog>

namespace Ui {
class PLSysCreator;
}

class PLSysCreator : public QDialog
{
    Q_OBJECT
    
public:
    explicit PLSysCreator(QWidget *parent = 0);
    ~PLSysCreator();
    void updateAlphabetList();

    
private:
    Ui::PLSysCreator *ui;

    PLSys * mPLsys;
    QStringListModel *modele;
    QStringList alphabet;

public slots:
    void addSymbol();
    void editSymbol();
    void addRule();
    void editRule();
    void addToAxiom();
    void removeFromAxiom();
    void setAxiom();

};

#endif // PLSYSCREATOR_H
