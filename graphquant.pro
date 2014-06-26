TEMPLATE = app
CONFIG += qxt
TARGET = graphquant
DEPENDPATH += .
INCLUDEPATH += .
RESOURCES = resources.qrc
QXT += core gui
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lmuparser
#RESOURCES = resources.qrc
#TRANSLATIONS = iching_cs.ts
#RC_FILE = icon.rc

HEADERS += main.h graph.h axis.h
SOURCES += main.cc graph.cc axis.cc
