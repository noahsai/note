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
        QRect rect = option.rect;
        QRect textRect;;
        LayerItem value = index.model()->data(index, Qt::EditRole).value<LayerItem>();

        QString layerName = value.note;
        QTextOption textOption;

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());

        if(value.isenable&&value.type==3)
        {
            deadline = value.tmpqstring;
        }
        else  deadline = value.time;
        if(deadline.indexOf(QRegularExpression(":.+:"))==-1){
            textRect.setRect(rect.x()+5, rect.y()+1, rect.width()-75, rect.height()-1);
        }
        else {
             textRect.setRect(rect.x()+5, rect.y()+1, rect.width()-105, rect.height()-1);
        }
//    //qDebug()<<textRect;


        textOption.setAlignment( Qt::AlignLeft | Qt::AlignTop);
//        textOption.setWrapMode(QTextOption::NoWrap);
        QFont font;
        font.setPixelSize(18);
        if (option.state & QStyle::State_Selected)     painter->setPen(QColor(255,255,255));
        else {
            if(textcolor.isEmpty())     painter->setPen(QColor(50,50,50));
            else painter->setPen(QColor(textcolor));
        }
        painter->setFont(font);
        painter->drawText(textRect, layerName, textOption);
//====================================================
        if(value.isnote)
        {
//            textRect.setRect(rect.x()+rect.width()-28, rect.y(), 26, 12);
//            painter->setPen(Qt::NoPen);
//            painter->setBrush(QBrush(QColor("#FFAEB9")));
//            if (!(option.state & QStyle::State_Selected)) painter->drawRect(textRect);
//            font.setPixelSize(9);
//            painter->setFont(font);
//            painter->setPen(QColor("white"));
//            textOption.setAlignment( Qt::AlignHCenter | Qt::AlignVCenter);
//            textOption.setWrapMode(QTextOption::NoWrap);
//            painter->drawText(textRect,QString("Note"),textOption);

            QPainterPath path;
            path.moveTo(rect.x()+rect.width()-20,rect.y());
            path.lineTo(rect.x()+rect.width(),rect.y());
            path.lineTo(rect.x()+rect.width(),rect.y()+20);
            painter->setPen(Qt::NoPen);
            if(tipbg.isEmpty())      painter->setBrush(QBrush(QColor("#FF82AB")));
            else    painter->setBrush(QBrush(QColor(tipbg)));
            if (!(option.state & QStyle::State_Selected)) painter->drawPath(path);

            textRect.setRect(rect.x()+rect.width()-10, rect.y(), 10, 12);

            font.setPixelSize(10);
            painter->setFont(font);
            if (option.state & QStyle::State_Selected)     painter->setPen(QColor(255,255,255));
            else {
                if(tipcolor.isEmpty())      painter->setPen(QColor("white"));
                else    painter->setPen(QColor(tipcolor));
            }
            textOption.setAlignment( Qt::AlignHCenter | Qt::AlignVCenter);
            textOption.setWrapMode(QTextOption::NoWrap);
            painter->drawText(textRect,QString("N"),textOption);
        }
        //=====================================================
        if(value.isenable&&value.type==3)
        {
            deadline = value.tmpqstring;
        }
        else  deadline = value.time;
        if(deadline.indexOf(QRegularExpression(":.+:"))==-1){
            textRect.setRect(rect.x()+rect.width()-72, rect.y(), 67, rect.height()-18);
        }
        else {
            textRect.setRect(rect.x()+rect.width()-102, rect.y(), 97, rect.height()-18);
        }
//    //qDebug()<<textRect;

        textOption.setAlignment( Qt::AlignRight | Qt::AlignBottom);
        textOption.setWrapMode(QTextOption::NoWrap);
//        if(deadline.indexOf(QRegularExpression(":.+:"))==-1) font.setPixelSize(26);
//        else
        font.setPixelSize(25);
        if (option.state & QStyle::State_Selected)     painter->setPen(QColor(255,255,255));
        else {
            if (option.state & QStyle::State_Selected)     painter->setPen(QColor(255,255,255));
            else   {
                if(timecolor.isEmpty())     painter->setPen(QColor("#1E90FF"));
                else    painter->setPen(QColor(timecolor));
            }
        }
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
            if(value.pre.length()==13)   deadline = "每天";
            else if(value.pre=="1,2,3,4,5") deadline = "工作日";
            else if(value.pre=="6,7") deadline = "休息日";
            else deadline = "星期" + change(value.pre);
            break;
        case 2:
            if(value.pre=="1")  deadline = "每天";
            else deadline = value.date+"起，每"+value.pre+"天";
            break;
        case 3:
            deadline = value.time+"起，每"+value.pre+"分钟";
            break;
        }
        textOption.setAlignment( Qt::AlignRight | Qt::AlignVCenter);
        textOption.setWrapMode(QTextOption::NoWrap);
        font.setPixelSize(12);

        if (!(option.state & QStyle::State_Selected)) {
            QFontMetrics text(font);
            QRect r = text.boundingRect(deadline);
            textRect.setRect(rect.x()+rect.width()-5-r.width()-8,rect.y()+rect.height()-18-2,text.boundingRect(deadline).width()+8,18);//-2是以为你要画圆滑矩形
            painter->setRenderHint(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            //if(datebg.isEmpty())    painter->setBrush(QBrush(QColor("#FF82AB")));
            if(datebg.isEmpty())    painter->setBrush(QBrush(QColor("#cccccc")));
            else    painter->setBrush(QBrush(QColor(datebg)));
            painter->drawRoundedRect(textRect,9,9);
        }
        textRect.setRect(rect.x()+5-4, rect.y()+rect.height()-18-2, rect.width()-10, 18);//-4是因为框-8

        if (option.state & QStyle::State_Selected)     painter->setPen(QColor(255,255,255));
        //else   painter->setPen(QColor(120,120,120));
        else {
            if(datecolor.isEmpty())    painter->setPen(QColor(255,255,255));
            else    painter->setPen(QColor(datecolor));
        }
         painter->setFont(font);

        painter->drawText(textRect, deadline, textOption);


    }
    else  if (index.column() == 0)
    {

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());
        QRect rect = option.rect;

        if(index.data(Qt::CheckStateRole)==Qt::Checked){
            if(onpic)   {
                QPixmap pix(cfgpath+"/on.png");
                pix = pix.scaled(30,30,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
                painter->drawImage(rect.x()+rect.width()/2-15,rect.y()+rect.height()/2-15,pix.toImage());
            }
            else    painter->drawImage(rect.x()+rect.width()/2-15,rect.y()+rect.height()/2-15,QImage(":/on.png"));
        }
        else {
            if(offpic)   {
                QPixmap pix(cfgpath+"/off.png");
                pix = pix.scaled(30,30,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
                painter->drawImage(rect.x()+rect.width()/2-15,rect.y()+rect.height()/2-15,pix.toImage());
            }
            else painter->drawImage(rect.x()+rect.width()/2-15,rect.y()+rect.height()/2-15,QImage(":/off.png"));
        }
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
    Q_UNUSED(event);
    Q_UNUSED(model);
    Q_UNUSED(index);
    Q_UNUSED(option);

    return false;
}

//QWidget *LayerItemDelegate::createEditor(QWidget *parent,
//    const QStyleOptionViewItem &option,
//    const QModelIndex &index) const
//{
////   // //qDebug() << "createEditor";
////    if (index.column() == 1) // value column
////    {
////        editnote* editor = new editnote(parent->parentWidget());
////      //  editor->setFixedHeight(60);
////        //editnote->setContentsMargins(48, 15, 50, 0);
////        return editor;
////    }
////    else return 0;  // no editor attached
//}

//void LayerItemDelegate::setEditorData(QWidget *e, const QModelIndex &index) const
//{
////    LayerItem value = index.model()->data(index, Qt::EditRole).value<LayerItem>();

////    editnote* editor = static_cast<editnote*>(e);
//   // editor->item = value ;
////    editor->setdata(value);
//  //  //qDebug() << "setEditorData";
//}

//void LayerItemDelegate::updateEditorGeometry(QWidget *editor,
//    const QStyleOptionViewItem &option, const QModelIndex & index ) const
//{
//    //editor->setGeometry(option.rect);
////    editor->setGeometry(editor->parentWidget()->rect());
//}

//void LayerItemDelegate::setModelData(QWidget *e, QAbstractItemModel *model,
//    const QModelIndex &index) const
//{
//  //  //qDebug() << "setModelData";
////    editnote* editor = static_cast<editnote*>(e);
////    QVariant data;
////    data.setValue(editor->item) ;
////    model->setData(index, data, Qt::EditRole);

//}
