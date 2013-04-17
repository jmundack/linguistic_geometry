#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T13:22:50
#
#-------------------------------------------------

QT       += core gui qt3support
QMAKE_CXXFLAGS += -g

TARGET = project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        twoD.h \
        twoD.cpp \
    trajectory.cpp \
    plot.cpp \
    latexgenerator.cpp \
    grammarofzones.cpp \
    searches.cpp

HEADERS  += mainwindow.h \
    trajectory.h \
    plot.h \
    latexgenerator.h \
    grammarofzones.h \
    searches.h

FORMS    += mainwindow.ui
