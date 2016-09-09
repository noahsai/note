#include "layertableview.h"

LayerTableView::LayerTableView(QWidget *parent)
: QTableView(parent)
{
    delegate = new LayerItemDelegate();
    model = new LayerTableModel();

    this->setContentsMargins(0, 0, 0, 0);
    this->setModel(model);
    this->setItemDelegate(delegate);

    this->horizontalHeader()->setStretchLastSection(true);
    this->horizontalHeader()->setHighlightSections(false);
    this->setFrameShape(QFrame::NoFrame);
//    this->setColumnWidth(0, 30);
//    this->setColumnWidth(1, 170);
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setVisible(false);
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    /*this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);*/
    this->setMouseTracking(true);//important

    //When click on the checkbox it will emit signal twice.Click on the cell emit only once.
    connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(itemClicked(const QModelIndex&)));
    readlist();
    connect(&timer,SIGNAL(timeout()),this,SLOT(timeout()));
    timer.setSingleShot(false);
    timer.start(1000);
}

LayerTableView::~LayerTableView()
{

}

void LayerTableView::mouseMoveEvent(QMouseEvent * event)
{

}



void LayerTableView::contextMenuEvent(QContextMenuEvent * event)
{
    QMenu *pMenu = new QMenu(this);
    QAction *pAddGroupAct = new QAction(tr("Delete"), pMenu);
    connect(pAddGroupAct ,SIGNAL(triggered(bool)),this,SLOT(deleteLayer()) );
    pMenu->addAction(pAddGroupAct);
    pMenu->popup(mapToGlobal(event->pos()));

}

void LayerTableView::addNewLayer()
{
    model->addItem();
    model->refreshModel();
    this->resizeRowsToContents();
}

void LayerTableView::itemClicked(const QModelIndex& index)
{
    if (index.isValid() )
    {
        //When click in column 0.
        if (index.column() == 0)
        {
            model->changeLayerVisibility(index);
       //     qDebug()<<"ok1"<<model->getSelecttedRow();
          //  model->setSelecttedRow(index.row());
            QModelIndex tmp = model->selecttedIndex(index.row());
         //   qDebug()<<"ok2";
            this->selectionModel()->select(tmp, QItemSelectionModel::Select);
      //      qDebug()<<"ok3";

        }
        //When click in column 1.
        else if (index.column() == 1)
        {
            model->setSelecttedRow(index.row());
        }
    }
}

void LayerTableView::deleteLayer()
{

    qDebug()<<"delete";
    model->deleteItem(model->getSelecttedRow());
    model->refreshModel();

    QModelIndex tmp = model->selecttedIndex(0);
    this->selectionModel()->select(tmp, QItemSelectionModel::Select);
    model->savelist();
}

void LayerTableView::setLayerSize(QSize s)
{
    layerSize = s;
}


bool LayerTableView::readlist()
{

    QFile file;
    QString path = QApplication::applicationDirPath()+"/notelist";
    qDebug()<<path;
    file.setFileName(path);
    QDataStream in(&file);
    if(file.open(QIODevice::ReadOnly))
    {
        LayerItem item;
        for(;!in.atEnd();)
        {
            in>>item;
            if(item.type>3) return false;
           qDebug()<<item.note<<item.type<<item.pre;
           model->addItem(item);
        }
        model->refreshModel();
        this->resizeRowsToContents();
        file.close();
        gettarkslist();
        return true;
    }
    else return false;
}



void LayerTableView::gettarkslist()
{
    int row = model->getlist().size();
    qDebug()<<"row="<<row;
    for(int i=0;i<row;i++)
    {
        LayerItem value = model->data(model->index(i,1) , Qt::EditRole).value<LayerItem>();

        if(!value.isenable) continue;
        qDebug()<<value.note;
        int type = value.type;
        switch(type)
        {
        case 0 :
            {
                QString date = value.date;
                QDate setday ;
                setday=QDate::fromString(date,"yyyy.M.d");
             //   qDebug()<<date<<setday.toString("yyyy.M.d");
                if(setday==QDate().currentDate())
                {
                    todayitem item;
                    item.index = model->index(0,i);
                    QTime settime;
                    settime=QTime::fromString(value.time,"h:m");
                    item.time = settime;
                    todaylist.append(item);
                    qDebug()<<"今天的事件，进入计时列表"<<value.time<<item.time;
                }
                break;
            }
        case 1 :
        {
            QString weeks = value.pre;
            QStringList weeklist = weeks.split(".");
            for(int i=0;i<weeklist.count();i++)
            {
                QString oneday = weeklist.at(i);
                int week = oneday.toInt();
                if(week==0) continue;
                if(week == QDate().currentDate().dayOfWeek())
                {
                    qDebug()<<"今天的星期事件，进入计时列表"<<value.time;;
                    todayitem item;
                    item.index = model->index(0,i);
                    QTime settime;
                    settime=QTime::fromString(value.time,"h:m");
                    item.time = settime;
                    todaylist.append(item);
                    break;
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

            if(days/someday == 0)
            {
                qDebug()<<"今天的几日事件，进入计时列表"<<value.time;;
                todayitem item;
                item.index = (model->index(0,i));
                QTime settime;
                settime=QTime::fromString(value.time,"h:m");
                item.time = settime;
                todaylist.append(item);
            }
            break;
        }
        case 3 :
        {
            todayitem item;
            item.index = model->index(0,i);
            QTime settime;
            settime=QTime::fromString(value.time,"h:m");
            item.time = settime;
            todaylist.append(item);
            break;
        }
        }
    }
}

void LayerTableView::timeout()
{

    for(int i=0;i<todaylist.size();i++)
    {
        QTime now = QTime::currentTime();
        if(todaylist.at(i).time.toString("h:m:s")==now.toString("h:m:s"))
        {
            qDebug()<<"bling!!!!!!";
            system("notify-send \"hi!note!\"");
            timer.stop();
        }
    }
}
