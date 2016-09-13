#include "layertableview.h"

LayerTableView::LayerTableView(QWidget *parent)
: QTableView(parent)
{
    setWindowFlags(Qt::Window |Qt::WindowTitleHint|Qt::WindowSystemMenuHint|Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);
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
    else {
//        this->show();
//        this->activateWindow();
        show_hide(!isActiveWindow());
    }
    savepos();
}


void LayerTableView::show_hide(bool sh)
{
    if(sh)
    {
        this->show();
        this->activateWindow();
    }
    else this->hide();
}

void LayerTableView::creattrayicon()
{
    quitAction = new QAction(tr("退出"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));//若触发了退出就退出程序
     trayIconMenu = new QMenu(this);//菜单
    trayIconMenu->addAction(quitAction);//把退出加到入菜单项
    trayIcon->setContextMenu(trayIconMenu);//设置托盘上下文菜单为trayIconMenu

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
    for(int i=0;i<todaylist.size();i++)
    {
        QTime now = QTime::currentTime();
        if(todaylist.at(i).time.toString("h:m:s")==now.toString("h:m:s"))
        {
            qDebug()<<"bling!!!!!!";
            QString iconpath = QApplication::applicationDirPath()+"/wei2.png";
            QString cmd ="notify-send -t 10000 -i '"+iconpath + "' ' ' \"╭(╯^╰)╮ 喂！\n"+ todaylist[i].note+"\"";
            //notify-send -t 时间 -i ‘图标地址’ ‘空标题’ "内容\n内容"
            system(cmd.toStdString().c_str());
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
    move(point);
}
