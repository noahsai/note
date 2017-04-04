#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T18:16:07
#
#-------------------------------------------------
#假如是其他系统（没有dtk环境的）可以将下面两句删了
#及将main.cpp里面 #include
CONFIG += link_pkgconfig
PKGCONFIG += dtkwidget dtkbase

#---------------------


QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = note
TEMPLATE = app


SOURCES += main.cpp\
    layertablemodel.cpp \
    layeritemdelegate.cpp \
    layertableview.cpp \
    edit.cpp \
    notify.cpp \
    notifyset.cpp \
    desktopnote.cpp

HEADERS  += \
    layertablemodel.h \
    layeritemdelegate.h \
    layertableview.h \
    edit.h \
    layeritem.h \
    notify.h \
    notifyset.h \
    desktopnote.h

FORMS    += \
    edit.ui \
    notify.ui \
    notifyset.ui \
    desktopnote.ui

RESOURCES += \
    res.qrc
