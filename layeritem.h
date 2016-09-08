#ifndef LAYERITEM_H
#define LAYERITEM_H
#include<QString>
struct LayerItem
{
    bool isenable;
    QString note;
    bool onetime;
    int type;
    QString date;
    QString time;
    QString pre;

};
Q_DECLARE_METATYPE(LayerItem);
#endif // LAYERITEM_H
