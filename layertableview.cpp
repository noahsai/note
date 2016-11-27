#include "layertableview.h"

LayerTableView::LayerTableView(QWidget *parent)
: QTableView(parent)
{
    setWindowFlags(Qt::Window |Qt::WindowCloseButtonHint);
    notifytime = 5000;
    notifymusic = ":/WA06.wav";
    notifyicon = ":/wei.png";
    sys_notify = false;
    notify = new Notify;

    delegate = new LayerItemDelegate();
    model = new LayerTableModel();
    this->setContentsMargins(0, 0, 0, 0);
    this->setModel(model);
    this->setItemDelegate(delegate);

    this->horizontalHeader()->setStretchLastSection(true);
    this->horizontalHeader()->setHighlightSections(false);
    this->setFrameShape(QFrame::NoFrame);
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setVisible(false);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setMouseTracking(true);//important
    //When click on the checkbox it will emit signal twice.Click on the cell emit only once.
    connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(itemClicked(const QModelIndex&)));
    //===============托盘图标============
    trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon(":/wei3.png"));
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(activate(QSystemTrayIcon::ActivationReason)));
    creattrayicon();
    trayIcon->show();
    //=========读取数据，启动计时---------------
    readlist();
    this->resizeColumnsToContents();//读取列表后调整size；
    this->resizeRowsToContents();//


    connect(&timer,SIGNAL(timeout()),this,SLOT(timeout()));
  //  timer.setSingleShot(false);
    timer.start(1000);
    readpos();
}

LayerTableView::~LayerTableView()
{
    savepos();
}

void LayerTableView::activate(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon:: Context) trayIcon->contextMenu()->show();
    else if(reason == QSystemTrayIcon::Trigger){
//        this->show();
//        this->activateWindow();
        show_hide(!isVisible());
    }
    savepos();
}


void LayerTableView::show_hide(bool sh)
{
    qDebug()<<"show_hide:"<<sh;
    if(sh)
    {
        this->show();
       // this->activateWindow();
    }
    else this->hide();
}

void LayerTableView::creattrayicon()
{
    QAction *set = new QAction(tr("设置"), this);
    connect(set, SIGNAL(triggered()), this, SLOT(newnotify()));
    quitAction = new QAction(tr("退出"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));//若触发了退出就退出程序
     trayIconMenu = new QMenu(this);//菜单
     trayIconMenu->addAction(set);//把设置加到入菜单项
     trayIconMenu->addAction(quitAction);//把退出加到入菜单项
    trayIcon->setContextMenu(trayIconMenu);//设置托盘上下文菜单为trayIconMenu

}

void LayerTableView::newnotify()
{
    set = new notifyset;
    set->setinit(notifytime,notifymusic,notifyicon);
    connect(set,SIGNAL(ok(int,QString,QString)),this,SLOT(setnotify(int,QString,QString)));
    set->show();
}

void LayerTableView::closeEvent(QCloseEvent *event)
{
    qDebug()<<"close to hide";
    this->hide();
    event->ignore();
}


void LayerTableView::contextMenuEvent(QContextMenuEvent * event)
{
    QMenu *pMenu = new QMenu(this);
    QAction *pAddGroupAct = new QAction(tr("添加"), pMenu);
    connect(pAddGroupAct ,SIGNAL(triggered(bool)),this,SLOT(addNewLayer()) );
    pMenu->addAction(pAddGroupAct);
    pAddGroupAct = new QAction(tr("删除"), pMenu);
    connect(pAddGroupAct ,SIGNAL(triggered(bool)),this,SLOT(deleteLayer()) );
    pMenu->addAction(pAddGroupAct);
    pMenu->popup(mapToGlobal(event->pos()));

}

void LayerTableView::addNewLayer()    //空白的就不保存了。
{
    model->addItem();
    qDebug()<<"model.additem ed";
    model->refreshModel();
    qDebug()<<"refreshmodel ed";

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
       //     QModelIndex tmp = model->selecttedIndex(index.row(),1);
        //    this->selectionModel()->select(tmp, QItemSelectionModel::Select);

        }
        //When click in column 1.
//        else if (index.column() == 1)
//        {
//            QModelIndex tmp = model->selecttedIndex(index.row(),0);
//            this->selectionModel()->select(tmp, QItemSelectionModel::Select);
//        }
    }
}

void LayerTableView::deleteLayer()
{
    qDebug()<<"delete";
    int currentrow = this->currentIndex().row();
    if(currentrow < 0 && model->rowCount(model->index(0,1))<=0) return;
    //model->deleteItem(model->getSelecttedRow());
    model->deleteItem(currentrow);
    model->refreshModel();
    model->savelist();
}

void LayerTableView::setLayerSize(QSize s)
{
    layerSize = s;
}


bool LayerTableView::readlist()
{

    QFile file;
    QString path = cfgpath+"/notelist";
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
           model->addItem(item,false);//由于多此重复操作，所以不updatetaskslist，后面手动update
        }
        model->refreshModel();
        this->resizeRowsToContents();
        file.close();
        model->update_taskslist();//完成后手动update
        return true;
    }
    else return false;
}



void LayerTableView::timeout()//不能没事件时停止timer，停止了进入下一日时无法获取事件；
{
    timer.stop();
    QList<todayitem> todaylist = model->get_tarkslist();
    int len = todaylist.size();
    for(int i=0;i<len;i++)
    {
        QTime now = QTime::currentTime();
        if(todaylist.at(i).time.toString("h:m:s")==now.toString("h:m:s"))
        {
            qDebug()<<"bling!!!!!!";
            if(sys_notify)
            {
                QString iconpath;
                if(iconpath.indexOf(":/wei")!=-1)  iconpath= QApplication::applicationDirPath()+"/wei.png";
                else iconpath = notifyicon;
                QString cmd ="notify-send -t "+QString().setNum(notifytime*-1)+" -i '"+iconpath + "' ' ' \"╭(╯^╰)╮ 喂！\n"+ todaylist[i].note+"\"";
                //notify-send -t 时间 -i ‘图标地址’ ‘空标题’ "内容\n内容"
                system(cmd.toStdString().c_str());
            }
            else {
                notify->message(todaylist[i].note);
                if(notify->isHidden()) notify->show();
            }
            this->clearSelection();
            if(todaylist[i].onetime)
            {
                this->clearSelection();
                itemClicked(todaylist[i].index);
            }
            else model->update_taskslist();

        }
    }
    timer.start(1000);
}

void LayerTableView::savepos()
{
    QSettings settings("ShengSoft", "Note");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("notifytime",notifytime);
    settings.setValue("notifymusic",notifymusic);
    settings.setValue("notifyicon",notifyicon);
    bool visi = this->isVisible();
    settings.setValue("isshow",visi);
    qDebug()<<visi;
}
void LayerTableView::readpos()
{
    QSettings settings("ShengSoft", "Note");
    resize(settings.value("size", QSize(360, 450)).toSize());//因为没得调大小，所以不要记录大小了
    int x=QApplication::desktop()->width()/2-width()/2;
    int y=QApplication::desktop()->height()/2-height()/2;
    QPoint point;
    point=settings.value("pos", QPoint(x, y)).toPoint();
    if(point.x()<0||point.x()>QApplication::desktop()->width()-20) point.setX(x);
    if(point.y()<0||point.y()>QApplication::desktop()->height()-20) point.setY(y);
    notifytime =  settings.value("notifytime",int(5000)).toInt();
    if(notifytime>0) sys_notify= false;
    else sys_notify=true;
    notifymusic = settings.value("notifymusic",QString(":/wei4.mp3")).toString();
    notifyicon = settings.value("notifyicon",QString(":/wei2.png")).toString();
    notify->init(notifytime,notifymusic,notifyicon);    //初始化NOTIFY
    move(point);
    bool visi = settings.value("isshow",true).toBool();
    show_hide(visi);

}


void LayerTableView::setnotify(int t,QString m,QString i)
{
    if(t<=0) sys_notify = true;
    else sys_notify = false;
    notifytime = t;
    if(!m.isEmpty()) notifymusic = m;
    if(!i.isEmpty()) notifyicon = i;
    notify->init(notifytime,notifymusic,notifyicon);
    qDebug()<<"setnotify ed";
    set->close();

}
