#-------------------------------------------------
#
# Project created by QtCreator 2014-01-20T15:10:06
#
#-------------------------------------------------

QT       += core gui testlib

TARGET = testm4calib
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += $(LOCALINST_DIR)/include
LIBPATH += $(LOCALINST_DIR)/lib
LIBS += -lCommonTUITypes -lTUIFrameworkClient -lTUIFrameworkCore

