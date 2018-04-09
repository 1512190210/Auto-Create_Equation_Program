#-------------------------------------------------
#
# Project created by QtCreator 2018-04-09T16:54:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Auto-Create_Equation_Program
TEMPLATE = app


SOURCES += main.cpp\
        query_interface.cpp \
    confirm_interface.cpp

HEADERS  += query_interface.h \
    confirm_interface.h

FORMS    += query_interface.ui \
    confirm_interface.ui

RC_FILE = study.rc
