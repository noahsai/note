#ifndef LAYERLISTVIEW_H
#define LAYERLISTVIEW_H

#include <QTableView>
#include <QMouseEvent>
#include <QDebug>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QContextMenuEvent>
#include <QMenu>
#include "layertablemodel.h"
#include "layeritemdelegate.h"
#include<layeritem.h>
#include<QTimer>
#include<QDate>
class LayerTableView : public QTableView
{
    Q_OBJECT

public:
    LayerTableView(QWidget *parent = 0);
    ~LayerTableView();
    void setLayerSize(QSize s);

public slots:
    void addNewLayer();
    void deleteLayer();

protected:
    void mouseMoveEvent(QMouseEvent * event);
    void contextMenuEvent(QContextMenuEvent * event);

private:
    LayerItemDelegate *delegate;
    LayerTableModel *model;
    QSize layerSize;
    QTimer timer;
    bool readlist();
    void gettarkslist();
    QList<todayitem> todaylist;


private slots:
    void itemClicked(const QModelIndex&);
    void timeout();

};

#endif // LAYERLISTVIEW_H

