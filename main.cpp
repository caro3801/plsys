#include <Convol/include/tools/Factories/RegisterAllConvolBlobtreeNodesT.h>
#include <Convol/include/tools/Factories/RegisterAllConvolFunctorsT.h>
#include <Convol/include/tools/Factories/RegisterAllConvolSkelPrimitivesT.h>
#include <config.h>
#include <QApplication>
#include <QDesktopWidget>
#include <iostream>
#include "window.h"
#include "functions.h"
using namespace std;

int main(int argc, char *argv[])
{

    bool graphic =true;
    if (graphic){

        // Init factories: To do only Once!!
            Convol::RegisterAllConvolFunctorsT<AppTraits>::RegisterNow();
            Convol::RegisterAllConvolSkelPrimitivesT<AppTraits>::RegisterNow();
            Convol::RegisterAllConvolBlobtreeNodesT<AppTraits>::RegisterNow();

            QApplication app(argc, argv);
            Window window;
            window.resize(window.sizeHint());
            int desktopArea = QApplication::desktop()->width() *
                    QApplication::desktop()->height();
            int widgetArea = window.width() * window.height();
            if (((float)widgetArea / (float)desktopArea) < 0.75f)
                window.show();
            else
                window.showMaximized();
            return app.exec();
    }else{
        PLSys * p=initglsystem();
        p->iterate();
        p->afficher(p->getPllist(),p->nbIterations);
    }


}


