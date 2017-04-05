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
    bool isnote;
    QString id;
    QString tmpqstring;
    QString html;
    QString color;
};
Q_DECLARE_METATYPE(LayerItem)



 inline QDataStream& operator<<(QDataStream& out, const LayerItem& item)
 {
     out<<item.isenable<<item.note<<item.onetime<<item.type<<item.date<<item.time<<item.pre<<item.isnote<<item.id<<item.tmpqstring<<item.html<<item.color;
     return out;
 }

 inline QDataStream& operator>>(QDataStream& in,LayerItem& item)
 {
     in>>item.isenable>>item.note>>item.onetime>>item.type>>item.date>>item.time>>item.pre>>item.isnote>>item.id>>item.tmpqstring>>item.html>>item.color;
    return in;
 }


 struct todayitem
 {
     QModelIndex index;
     QTime time;
     QString note;
     bool onetime;
     bool isnote;
 };

// struct WindowTheme
// {
//    QString selectedcolor;
//    QString textcolor;
//    QString timecolor;
//    QString datecolor;
//    QString datebg;
//    QString tipcolor;
//    QString tipbg;
//    bool onpic;
//    bool offpic;
// };
// Q_DECLARE_METATYPE(WindowTheme)

// inline QDataStream& operator<<(QDataStream& out, const WindowTheme& item)
// {
//     out<<item.selectedcolor<<item.textcolor<<item.timecolor<<item.datecolor<<item.datebg<<item.tipcolor<<item.tipbg<<item.onpic<<item.offpic;
//     return out;
// }

// inline QDataStream& operator>>(QDataStream& in,WindowTheme& item)
// {
//     in>>item.selectedcolor>>item.textcolor>>item.timecolor>>item.datecolor>>item.datebg>>item.tipcolor>>item.tipbg>>item.onpic>>item.offpic;
//    return in;
// }


 enum DOING {
     AddItem,
     AddItemNoUpdateList,
     EditItem,
     EditbyNote,
     Nothing
 };


#endif // LAYERITEM_H
