#ifndef LAYERITEM_H
#define LAYERITEM_H
#include<QString>
#include <QDataStream>
#include<QFile>
#include<QApplication>
#include<QModelIndex>
#include<QTime>
struct LayerItem
{

    bool isenable;
    QString note;
    bool onetime;
    int type;
    QString date;
    QString time;
    QString pre;
    bool tmpbool;
    int tmpint;
    QString tmpqstring;
};
Q_DECLARE_METATYPE(LayerItem)



 inline QDataStream& operator<<(QDataStream& out, const LayerItem& item)
 {
     out<<item.isenable<<item.note<<item.onetime<<item.type<<item.date<<item.time<<item.pre<<item.tmpbool<<item.tmpint<<item.tmpqstring;
     return out;
 }

 inline QDataStream& operator>>(QDataStream& in,LayerItem& item)
 {
     in>>item.isenable>>item.note>>item.onetime>>item.type>>item.date>>item.time>>item.pre>>item.tmpbool>>item.tmpint>>item.tmpqstring;
    return in;
 }


 struct todayitem
 {
     QModelIndex index;
     QTime time;
     QString note;
     bool onetime;
 };

#endif // LAYERITEM_H
