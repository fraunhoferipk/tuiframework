#-------------------------------------------------
#
# Project created by QtCreator 2014-02-24T18:21:56
#
#-------------------------------------------------

QT       += core gui testlib

TARGET = testm4calib2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += $(LOCALINST_DIR)/include
LIBPATH += $(LOCALINST_DIR)/lib
LIBS += -lCommonTUITypes -lTUIFrameworkClient -lTUIFrameworkCore
