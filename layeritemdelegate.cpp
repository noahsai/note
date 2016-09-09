#include "layeritemdelegate.h"

QString change(QString& in)
{
    QString out = in;
    out = out.replace("1","一");
    out = out.replace("2","二");
    out = out.replace("3","三");
    out = out.replace("4","四");
    out = out.replace("5","五");
    out = out.replace("6","六");
    out = out.replace("7","七");
    return out;
}




LayerItemDelegate::LayerItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

LayerItemDelegate::~LayerItemDelegate()
{

}



void LayerItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{


    if (index.column() == 1) // value column
    {
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());
        QRect rect = option.rect;


        QRect textRect(rect.x()+2, rect.y(), rect.width(), rect.height()/3*2);

        LayerItem value = index.model()->data(index, Qt::EditRole).value<LayerItem>();

        QString layerName = value.note;
        QTextOption textOption = Qt::AlignLeft | Qt::AlignVCenter;
        textOption.setWrapMode(QTextOption::NoWrap);
        QFont font;
        font.setPixelSize(20);
        painter->setFont(font);
        painter->drawText(textRect, layerName, textOption);
//================================
        QString deadline;
        int type = value.type;
        switch(type)
        {
        case 0:
            deadline = value.date+"  "+value.time;
            break;
        case 1:
            deadline = "星期" + change(value.pre);
            break;
        case 2:
            deadline = value.date+"起，每"+value.pre+"天";
            break;
        case 3:
            deadline = value.time+"起，每"+value.pre+"分钟";
            break;
        }
        textRect.setRect(rect.x()+2, rect.y()+rect.height()-rect.height()/2, rect.width()-8, rect.height()/2-4);
        textOption = Qt::AlignRight | Qt::AlignBottom;
        textOption.setWrapMode(QTextOption::NoWrap);
        font.setPixelSize(14);
        painter->setFont(font);
        painter->drawText(textRect, deadline, textOption);
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}


bool LayerItemDelegate::editorEvent(QEvent * event,
    QAbstractItemModel * model,
    const QStyleOptionViewItem & option,
    const QModelIndex & index)
{
    return false;
}

QWidget *LayerItemDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    qDebug() << "createEditor";
    if (index.column() == 1) // value column
    {
        edit* editor = new edit(parent);
        editor->setFixedHeight(60);
        //edit->setContentsMargins(48, 15, 50, 0);
        return editor;
    }
    else return 0;  // no editor attached
}

void LayerItemDelegate::setEditorData(QWidget *e, const QModelIndex &index) const
{
    LayerItem value = index.model()->data(index, Qt::EditRole).value<LayerItem>();

    edit* editor = static_cast<edit*>(e);
   // editor->item = value ;
    editor->setdata(value);
    qDebug() << "setEditorData";
}

void LayerItemDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    editor->setGeometry(option.rect);
}

void LayerItemDelegate::setModelData(QWidget *e, QAbstractItemModel *model,
    const QModelIndex &index) const
{
    qDebug() << "setModelData";
    edit* editor = static_cast<edit*>(e);
    QVariant data;
    data.setValue(editor->item) ;
    model->setData(index, data, Qt::EditRole);

}
