######################################################################
# Automatically generated by qmake (2.01a) ven. mai 31 11:10:41 2013
######################################################################
QT+= core gui opengl

TEMPLATE = app
TARGET = PLSys

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += . \
               GUI \
               LSys \
               OM \
               GA \

# Input
HEADERS +=  icosphere.h \
            mainwindow.h \
            GA/PLSysGenome.h \
            GA/SymbolChromosome.h \
            GA/SPopulation.h \
            GA/SymbolPChromosome.h \
            GUI/CellPLSys.h \
            GUI/glwidget.h \
            LSys/functions.h \
            LSys/param.h \
            LSys/plsys.h \
            LSys/rule.h \
            LSys/symbol.h \
            LSys/turtle.h \
            LSys/turtlecore.h \
            LSys/symbolforward.h \
            LSys/symbolangle.h \
            LSys/symbolstack.h \
            LSys/symbolfactory.h \
    GA/ColorChromosome.h

SOURCES +=  mainwindow.cpp \
            icosphere.cpp \
            main.cpp \
            GA/PLSysGenome.cpp \
            GA/SymbolChromosome.cpp \
            GA/SPopulation.cpp \
            GA/SymbolPChromosome.cpp \
            GUI/CellPLSys.cpp \
            GUI/glwidget.cpp \
            LSys/plsys.cpp \
            LSys/rule.cpp \
            LSys/symbol.cpp \
            LSys/turtle.cpp \
            LSys/turtlecore.cpp \
            LSys/symbolforward.cpp \
            LSys/symbolangle.cpp \
            LSys/symbolstack.cpp \
            LSys/symbolfactory.cpp \
    GA/ColorChromosome.cpp

FORMS += \
            mainwindow.ui


INCLUDEPATH += /opt/local/include
LIBS += -L/opt/local/lib
LIBS += -lboost_system-mt -lboost_filesystem-mt

INCLUDEPATH += /usr/local/include/OpenMesh
LIBS += -L/usr/local/lib/OpenMesh
unix: LIBS += -lOpenMeshCore


unix:!macx: LIBS += -L$$PWD/../build-convol/ -lconvol

INCLUDEPATH += $$PWD/../convol
DEPENDPATH += $$PWD/../convol

unix:!macx: PRE_TARGETDEPS += $$PWD/../build-convol/libconvol.a



unix:!macx: LIBS += -L$$PWD/../build-OOGA/ -lOOGA

INCLUDEPATH += $$PWD/../OOGA
DEPENDPATH += $$PWD/../OOGA

unix:!macx: PRE_TARGETDEPS += $$PWD/../build-OOGA/libOOGA.a




#unix:!macx: LIBS += -L$$PWD/../OpenMesh-2.3.1/build/Build/lib/OpenMesh/ -lOpenMeshCore

#INCLUDEPATH += $$PWD/../OpenMesh-2.3.1/src
#DEPENDPATH += $$PWD/../OpenMesh-2.3.1/build/Build/lib/OpenMesh

#unix:!macx: PRE_TARGETDEPS += $$PWD/../OpenMesh-2.3.1/build/Build/lib/OpenMesh/libOpenMeshCore.a

win32: LIBS += -lOpenMeshCore
