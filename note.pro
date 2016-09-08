#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T18:16:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = note
TEMPLATE = app


SOURCES += main.cpp\
        note.cpp \
    layertablemodel.cpp \
    layeritemdelegate.cpp \
    layertableview.cpp \
    edit.cpp

HEADERS  += note.h \
    layertablemodel.h \
    layeritemdelegate.h \
    layertableview.h \
    edit.h \
    layeritem.h

FORMS    += note.ui \
    edit.ui
