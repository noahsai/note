#ifndef LAYERITEMDELEGATE_H
#define LAYERITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QDebug>
#include <QPainter>
#include <edit.h>
#include<layeritem.h>
#include<QStaticText>

extern QString cfgpath;//全局变量

extern QString selectedcolor;
extern QString textcolor;
extern QString timecolor;
extern QString datecolor;
extern QString datebg;
extern QString tipcolor;
extern QString tipbg;
extern bool onpic;
extern bool offpic;


class LayerItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    LayerItemDelegate(QObject *parent=0);
    ~LayerItemDelegate();


    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
//        const QModelIndex &index) const;
    bool editorEvent(QEvent * event,
        QAbstractItemModel * model,
        const QStyleOptionViewItem & option,
        const QModelIndex & index);
//    void setEditorData(QWidget *editor, const QModelIndex &index) const;
//    void setModelData(QWidget *editor, QAbstractItemModel *model,
//        const QModelIndex &index) const;
//    void updateEditorGeometry(QWidget *editor,
//        const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
};

#endif // LAYERITEMDELEGATE_H
