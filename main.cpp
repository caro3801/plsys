#include <Convol/include/tools/Factories/RegisterAllConvolBlobtreeNodesT.h>
#include <Convol/include/tools/Factories/RegisterAllConvolFunctorsT.h>
#include <Convol/include/tools/Factories/RegisterAllConvolSkelPrimitivesT.h>
#include <config.h>
#include <QApplication>
#include <QDesktopWidget>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

int main(int argc, char *argv[])
{



      Convol::RegisterAllConvolFunctorsT<AppTraits>::RegisterNow();
        Convol::RegisterAllConvolSkelPrimitivesT<AppTraits>::RegisterNow();
        Convol::RegisterAllConvolBlobtreeNodesT<AppTraits>::RegisterNow();
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();


}


