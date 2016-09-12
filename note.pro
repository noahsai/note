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
    layertablemodel.cpp \
    layeritemdelegate.cpp \
    layertableview.cpp \
    edit.cpp

HEADERS  += \
    layertablemodel.h \
    layeritemdelegate.h \
    layertableview.h \
    edit.h \
    layeritem.h

FORMS    += \
    edit.ui

RESOURCES += \
    res.qrc
