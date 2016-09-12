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
    out = out.replace("7","日");
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
        QString deadline;

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());
        QRect rect = option.rect;


        QRect textRect(rect.x()+8, rect.y(), rect.width()-100, rect.height());

        LayerItem value = index.model()->data(index, Qt::EditRole).value<LayerItem>();

        QString layerName = value.note;
        QTextOption textOption = Qt::AlignLeft | Qt::AlignVCenter;
//        textOption.setWrapMode(QTextOption::NoWrap);
        QFont font;
        font.setPixelSize(18);
        if (option.state & QStyle::State_Selected)     painter->setPen(QColor(255,255,255));
        else painter->setPen(QColor(0,0,0));
        painter->setFont(font);
        painter->drawText(textRect, layerName, textOption);
//====================================================
        if(value.isenable&&value.type==3)
        {
            deadline = value.tmpqstring;
        }
        else  deadline = value.time;
        textRect.setRect(rect.x()+rect.width()-100, rect.y(), 92, rect.height()-20);
        textOption = Qt::AlignRight | Qt::AlignVCenter;
        textOption.setWrapMode(QTextOption::NoWrap);
        if(deadline.indexOf(QRegularExpression(":.+:"))==-1) font.setPixelSize(26);
        else font.setPixelSize(24);
//        painter->setPen(QColor(0,0,0));
        painter->setFont(font);
        painter->drawText(textRect, deadline, textOption);
//================================
        int type = value.type;
        switch(type)
        {
        case 0:
        {
            deadline = value.date;
            int pos = deadline.indexOf(QRegularExpression("\\d{1,2}$"));
            switch (pos)
            {
            case 3:
            case 2:
              deadline = "每年 "+deadline;
                break;
            case 0:
                deadline = "每月 "+deadline+" 号";
                break;
            default:
                break;
            }
            break;
        }
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
        textRect.setRect(rect.x()+8, rect.y()+rect.height()-20, rect.width()-16, 20);
        textOption = Qt::AlignRight | Qt::AlignVCenter;
        textOption.setWrapMode(QTextOption::NoWrap);
        font.setPixelSize(13);
        if (option.state & QStyle::State_Selected)     painter->setPen(QColor(255,255,255));
        else   painter->setPen(QColor(120,120,120));
        painter->setFont(font);
        painter->drawText(textRect, deadline, textOption);

    }
    else  if (index.column() == 0)
    {

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());
        QRect rect = option.rect;

        if(index.data(Qt::CheckStateRole)==Qt::Checked)
            painter->drawImage(rect.x()+rect.width()/2-15,rect.y()+20,QImage(":/on.png"));
        else painter->drawImage(rect.x()+rect.width()/2-15,rect.y()+20,QImage(":/off.png"));
    }
//    else
//    {
//        QStyledItemDelegate::paint(painter, option, index);
//    }
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
   // qDebug() << "createEditor";
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
  //  qDebug() << "setEditorData";
}

void LayerItemDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    editor->setGeometry(option.rect);
}

void LayerItemDelegate::setModelData(QWidget *e, QAbstractItemModel *model,
    const QModelIndex &index) const
{
  //  qDebug() << "setModelData";
    edit* editor = static_cast<edit*>(e);
    QVariant data;
    data.setValue(editor->item) ;
    model->setData(index, data, Qt::EditRole);

}
