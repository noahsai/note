#include "layertablemodel.h"

LayerTableModel::LayerTableModel(QObject *parent)
: QAbstractTableModel(parent)
{
   // selectedRow = 0;//暂时不用，感觉对于我的程序没用。

}

LayerTableModel::~LayerTableModel()
{

}

QVariant LayerTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int column = index.column();
    if (column == 0)
    {
        if(role ==  Qt::CheckStateRole)
        {
            return layerList.at(index.row()).isenable ? Qt::Checked : Qt::Unchecked;
        }
        if (role == Qt::SizeHintRole)
        {
            return QSize(30, 60);
        }
    }
    else
    {
        if (role == Qt::EditRole)
        {
            QVariant data;
            data.setValue(layerList.at(index.row())) ;
            return data;
        }
        else if (role == Qt::DisplayRole)
        {
            return QVariant(layerList.at(index.row()).note);
        }


        else if (role == Qt::SizeHintRole)
        {
            return QSize(470, 60);
        }
        else if (role == Qt::TextAlignmentRole)
        {
            return int(Qt::AlignVCenter);
        }
    }

    return QVariant();
}

int LayerTableModel::rowCount(const QModelIndex &parent) const
{
    return (parent.isValid() && parent.column() != 0) ? 0 : layerList.size();
}

int LayerTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant LayerTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        return QString::number(section);
    //if (role == Qt::DecorationRole)
        //return QVariant::fromValue(services);
    return QAbstractItemModel::headerData(section, orientation, role);
}

Qt::ItemFlags LayerTableModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return 0;

    if (index.column() == 0)
        return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;

    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool LayerTableModel::setData(const QModelIndex &index, const
    QVariant &val, int role)
{
    if (!index.isValid())
    {
        return false;
    }

    if (role == Qt::CheckStateRole && val.type() == QVariant::Bool)
    {
        layerList[index.row()].isenable = val.toBool();
        emit(dataChanged(index, index));
        update_taskslist();
        savelist();
        return true;
    }
    else if (role == Qt::EditRole && index.column() == 1)
    {
        LayerItem va = val.value<LayerItem>();
        layerList.replace(index.row(),va);
        emit(dataChanged(index, index));
        update_taskslist();
        savelist();
        return true;
    }
    return false;;
}

void LayerTableModel::changeLayerVisibility(const QModelIndex& index)
{
    qDebug()<<"changeLayerVisibility";
    if (index.isValid()&&index.column() == 0)
    {
        setData(index, !(layerList.at(index.row()).isenable), Qt::CheckStateRole);//里面保存了


    }
}

void LayerTableModel::deleteItem(int index)
{
    qDebug()<<"remove:"<<index;
    if(index==-1) return;
    QList<LayerItem>::iterator it = layerList.begin();
    layerList.erase(it + index);
    update_taskslist();

}

void LayerTableModel::addItem()
{
    LayerItem item;

        item.note = QString("空白事件。");

    item.isenable = false;

    layerList.append(item);
    //this->insertRow()
    //emit(dataChanged(index, index));
    qDebug()<<layerList.size();
}


void LayerTableModel::addItem(LayerItem& add,bool update_tasks_list)
{
    LayerItem item = add;
    layerList.append(item);
    //this->insertRow()
    //emit(dataChanged(index, index));
    qDebug()<<layerList.size();
    savelist();
    if(update_tasks_list) update_taskslist();

}

void LayerTableModel::refreshModel()
{
    beginResetModel();
    endResetModel();
    //emit updateCount(this->rowCount(QModelIndex()));
}

QModelIndex LayerTableModel::selecttedIndex(int row)
{
    QModelIndex m= this->createIndex(row, 1);
    qDebug()<<"selected:"<<row;
    return m;
}

//void LayerTableModel::setSelecttedRow(int row)
//{
//    qDebug()<<"selected row:"<<row;
//    selectedRow = row;
//}

//int LayerTableModel::getSelecttedRow() const
//{
//    return selectedRow;
//}

bool LayerTableModel::savelist()
{
    QFile file;
    QString path = QApplication::applicationDirPath()+"/notelist";
    file.setFileName(path);
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        for(int i=0;i<layerList.count();i++)
        {
            out<<layerList[i];
        }

        file.close();
        return true;
    }
    else return false;
}


void LayerTableModel::update_taskslist()
{
    qDebug()<<"updatelist";
    int row = layerList.size();
    for(int i=0;i<row;i++)
    {
        LayerItem value = layerList[i];

        if(!value.isenable) continue;
       // qDebug()<<value.note;
        int type = value.type;
        switch(type)
        {
        case 0 :
            {
                QString date = value.date;
                QDate setday ;
                setday=QDate::fromString(date,"yyyy.M.d");
             //   qDebug()<<date<<setday.toString("yyyy.M.d");
                if((setday==QDate().currentDate())||(( setday==QDate().currentDate().addDays(1))&&value.time=="00:00" ))
                {
                    todayitem item;
                    item.index =index(i,0);
                    QTime settime;
                    settime=QTime::fromString(value.time,"h:m");
                    if(settime<QTime::currentTime() && ( !(( setday==QDate().currentDate().addDays(1))&&value.time=="00:00" )))
                        break;
                    item.time = settime;
                    item.onetime = value.onetime;
                    todaylist.append(item);
                    qDebug()<<"今天的事件，进入计时列表"<<value.time<<item.time;
                }
                break;
            }
        case 1 :
        {
            QString weeks = value.pre;
            QStringList weeklist = weeks.split(",");
            for(int i=0;i<weeklist.count();i++)
            {
                QString oneday = weeklist.at(i);
                int week = oneday.toInt();
                if(week==0) continue;//返回0有可能是toInt转换失败，反正没有星期0所以不用判断0是怎样来。
                qDebug()<<"week:"<<week%QDate().currentDate().dayOfWeek();
                if((week == QDate().currentDate().dayOfWeek())||(((week%QDate().currentDate().dayOfWeek())==1)&&value.time=="00:00" ))
                {

                    QTime settime;
                    settime=QTime::fromString(value.time,"h:m");
                    if(settime<QTime::currentTime() && ( !(((week%QDate().currentDate().dayOfWeek())==1)&&value.time=="00:00" )))
                        continue;
                    todayitem item;
                    item.index = index(i,0);

                    item.time = settime;
                    item.onetime = value.onetime;
                    todaylist.append(item);
                    qDebug()<<"今天的星期事件，进入计时列表"<<value.time;;
                    break;//每天只有一个时间，所以找到一个有效日子就足够了。
                }

            }

            break;
        }
        case 2 :
        {
            QString date2 = value.date;
            QDate setday ;
            setday=QDate::fromString(date2,"yyyy.M.d");
            QDate today;
            today = today.currentDate();
            int someday = value.pre.toInt();
            int days = setday.daysTo(today);

            if((days%someday == 0)||((days%someday)==1&&value.time=="00:00"))
            {
                qDebug()<<"今天的几日事件，进入计时列表"<<value.time;;
                todayitem item;
                item.index = (index(i,0));
                QTime settime;
                settime=QTime::fromString(value.time,"h:m");
                if(settime<QTime::currentTime() && ( !((days%someday)==1&&value.time=="00:00")))
                    break;
                item.time = settime;
                item.onetime = value.onetime;

                todaylist.append(item);
            }
            break;
        }
        case 3 :
        {
            todayitem item;
            item.index = index(i,0);
            QTime settime;
            settime=QTime::fromString(value.time,"h:m");
            item.time = settime;
           item.onetime = value.onetime;

            todaylist.append(item);
            break;
        }
        }
    }
}

QList<todayitem> LayerTableModel::get_tarkslist()
{
    return todaylist;
}
