#include "layertableview.h"


//==Theme==
QString selectedcolor="";
QString textcolor="";
QString timecolor="";
QString datecolor="";
QString datebg="";
QString tipcolor="";
QString tipbg="";
bool onpic=false;
bool offpic=false;
//=============

LayerTableView::LayerTableView(QWidget *parent)
: QTableView(parent)
{
    connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(edititem(QModelIndex)));
    setWindowFlags(Qt::Window |Qt::WindowCloseButtonHint);
    this->setColumnWidth(0,45);//设置第一列列宽
    set = nullptr;
    notify = new Notify;//readpos()里会初始化各参数
    doing = Nothing;
    editor = nullptr;
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
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/wei4.png"));
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(activate(QSystemTrayIcon::ActivationReason)));
    creattrayicon();
    trayIcon->show();
    //=========读取数据，启动计时---------------
    readlist();
    this->resizeColumnsToContents();//读取列表后调整size；
    this->resizeRowsToContents();//


    connect(&timer,SIGNAL(timeout()),this,SLOT(timeout()));
  //  timer.setSingleShot(false);
    timer.start(800);
    readtheme();
    readpos();//各种初始化
}

LayerTableView::~LayerTableView()
{
    savepos();
    savetheme();
}

void LayerTableView::activate(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon:: Context) trayIcon->contextMenu()->show();
    else if(reason == QSystemTrayIcon::Trigger){
//        this->show();
//        this->activateWindow();
        show_hide(!isVisible());
        savepos();
    }
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
    if(set!=nullptr) return;
    set = new notifyset;
    set->setWindowIcon(QIcon(":/wei2.png"));
    set->setinit(notifytime,notifymusic,notifyicon);
    connect(set,SIGNAL(ok(int,QString,QString)),this,SLOT(setnotify(int,QString,QString)));
    connect(set,SIGNAL(selectcolorset()),this,SLOT(setselectcolor()));
    connect(set,SIGNAL(destroyed(QObject*)),this,SLOT(settingclose()));
    connect(set,SIGNAL(freshtheme()),this,SLOT(freshtheme()));

    set->show();
}

void LayerTableView::freshtheme(){
    this->resizeRowsToContents();
}

void LayerTableView::settingclose()
{
    set = nullptr;
}

void LayerTableView::setselectcolor(){
   if(selectedcolor.isEmpty()) {
       setStyleSheet("QTableView {\
                                selection-background-color :  qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(125, 214, 221, 255), stop:0.505618 rgba(128, 199, 223, 255), stop:1 rgba(108, 201, 230, 255));\
                                             gridline-color:rgb(230, 230, 230) ;}");
    }
    else {
        QString s ="QTableView {selection-background-color :";
        setStyleSheet(s + selectedcolor + ";}");
    }
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
    connect(pAddGroupAct ,SIGNAL(triggered(bool)),this,SLOT(additem()) );
    pMenu->addAction(pAddGroupAct);
    if(this->indexAt(event->pos()).isValid())
    {
        pAddGroupAct = new QAction(tr("便签开/关"), pMenu);
        connect(pAddGroupAct ,SIGNAL(triggered(bool)),this,SLOT(changenote()) );
        pMenu->addAction(pAddGroupAct);
        pAddGroupAct = new QAction(tr("删除"), pMenu);
        connect(pAddGroupAct ,SIGNAL(triggered(bool)),this,SLOT(deleteLayer()) );
        pMenu->addAction(pAddGroupAct);
    }
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
    qDebug()<<"Delete items";
//    int currentrow = this->currentIndex().row();
//    if(currentrow < 0 && model->rowCount(model->index(0,1))<=0) return;
//    //model->deleteItem(model->getSelecttedRow());
//    QModelIndex index= model->index(this->currentIndex().row(),1);
//    LayerItem item = model->data(index,Qt::EditRole).value<LayerItem>();
//    if(hasitem(item.id))
//    {
//        desktopNote *note = notelist.value(item.id);
//        note->close();
//        note = nullptr;
//        notelist.remove(item.id);
//    }
//    QFile f(cfgpath+"/"+ item.id);//删除配置文件
//    if(f.exists()) f.remove();
//    model->deleteItem(currentrow);
//    model->refreshModel();
//    model->savelist();

    QModelIndexList seledlist = this->selectedIndexes();
    QModelIndex index;
    LayerItem item;
    QStringList removelist;
    for(int i=0;i<seledlist.length();i++)
    {
        index = seledlist.at(i);
        if(index.column()==1){
            item = model->data(index,Qt::EditRole).value<LayerItem>();
            removelist.append(item.id);
            qDebug()<<"found need delete item"<<item.id;
        }
    }
    qDebug()<<removelist.length()<<"need remove";
    QString id;
    for(int i=0;i<removelist.count();i++)
    {
        qDebug()<<"--------\nDeleting item";
        id=removelist.at(i);
        if(hasitem(id))
        {
            qDebug()<<"close item note"<<id;
            desktopNote *note = notelist.value(id);
            note->close();
            note = nullptr;
            notelist.remove(id);
            qDebug()<<id<<"note closed";

        }
        QFile f(cfgpath+"/"+ id);//删除配置文件
        f.close();
        if(f.exists()) {
            qDebug()<<"to delete note cfg"<<f.fileName();
            if(f.remove()) qDebug()<<"deleted cfg";
        }
        model->deleteItem(id );
        qDebug()<<"Item deleted"<<id;
    }
    model->refreshModel();
    model->update_taskslist();//delete完要手动updatelist
    model->savelist();
    qDebug()<<"All Deleted";

}

void LayerTableView::setLayerSize(QSize s)
{
    layerSize = s;
}


bool LayerTableView::readlist()
{

    QFile file;
    QString path = cfgpath+"/notelist";
    qDebug()<<"readlist"<<path;
    file.setFileName(path);
    QDataStream in(&file);
    if(file.open(QIODevice::ReadOnly))
    {
        LayerItem item;
        for(;!in.atEnd();)
        {
            doing = AddItemNoUpdateList;//使editfinished里不update
            in>>item;
            if(item.type>3) return false;
           qDebug()<<item.isnote<<item.isnote<<item.note<<item.date<<item.tmpqstring<<item.pre<<item.onetime<<item.type<<item.time;
           editfinished(item);//配合doing = AddItemNoUpdateList;
        }
        model->refreshModel();
        this->resizeRowsToContents();
        file.close();
        model->update_taskslist();//完成后手动update
        return true;
    }
    file.close();
    return false;
}



void LayerTableView::timeout()//不能没事件时停止timer，停止了进入下一日时无法获取事件；
{
    timer.stop();
    QList<todayitem> todaylist = model->get_tarkslist();
    int len = todaylist.size();
    for(int i=0;i<len;i++)
    {
        QTime now = QTime::currentTime();
        if(todaylist.at(i).time.toString("hh:mm:ss")==now.toString("hh:mm:ss"))
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
            if( todaylist[i].onetime )
            {
                itemClicked(todaylist[i].index);//里面会updatetasklist
                QModelIndex index=model->index(todaylist[i].index.row(),1);
                LayerItem i = model->data(index,Qt::EditRole).value<LayerItem>();
                if(hasitem(i.id))
                {
                    desktopNote *note;
                    note = notelist.value(i.id);
                    note->setdata(i);//设置新数据
                    note->initnote();//刷新便签
                }
            }
            else {
                model->update_taskslist();
            }
        }
    }
    timer.start(800);
}

void LayerTableView::savepos()
{
    QString cfg = cfgpath+"/noetwei";
    QSettings settings(cfg,QSettings::NativeFormat);
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("notifytime",notifytime);
    settings.setValue("notifymusic",notifymusic);
    settings.setValue("notifyicon",notifyicon);
    bool visi = this->isVisible();
    settings.setValue("isshow",visi);
    qDebug()<<"LayerTableView::savepos()"<<visi;
}
void LayerTableView::readpos()
{
    QString cfg = cfgpath+"/noetwei";
    QSettings settings(cfg,QSettings::NativeFormat);
    resize(settings.value("size", QSize(360, 450)).toSize());//因为没得调大小，所以不要记录大小了
    int x=QApplication::desktop()->width()/2-width()/2;
    int y=QApplication::desktop()->height()/2-height()/2;
    QPoint point;
    point=settings.value("pos", QPoint(x, y)).toPoint();
    if(point.x()<0||point.x()>QApplication::desktop()->width()-20) point.setX(x);
    if(point.y()<0||point.y()>QApplication::desktop()->height()-20) point.setY(y);
    notifytime =  settings.value("notifytime",int(10000)).toInt();
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


//==================================================================================

void LayerTableView:: edititem(const QModelIndex& index){
    if (index.column() == 0) return;
    if(editor!=nullptr) return;//必须先判断
    doing = EditItem;//再设doing
    editor = new editnote();
    editor->setWindowIcon(QIcon(":/wei2.png"));
    editor->setWindowFlags(Qt::CustomizeWindowHint|Qt::Window|Qt::WindowTitleHint);//使用窗口管理器
    editor->setWindowTitle("编辑");
    //editor->setWindowModality(Qt::ApplicationModal);//独占，其他窗口不能操作
    //editor->setAttribute(Qt::WA_DeleteOnClose);
    editor->resize(200,350);
    editor->move(QApplication::desktop()->width()/2-editor->width()/2,QApplication::desktop()->height()/2-editor->height()/2);
    LayerItem item = model->data(index,Qt::EditRole).value<LayerItem>();
    editor->setdata(item);
    connect(editor,SIGNAL(setfinished(LayerItem,QPixmap)),this,SLOT(editfinished(LayerItem)));//利用noteedited()，不需要独占
    connect(editor,SIGNAL(editcancel()),this,SLOT(editcancel()));
    editor->show();

}

void LayerTableView:: additem(){
    if(editor!=nullptr) return;//必须先判断
    doing = AddItem;
    editor = new editnote();
    editor->setWindowIcon(QIcon(":/wei2.png"));
    editor->setWindowFlags(Qt::CustomizeWindowHint|Qt::Window|Qt::WindowTitleHint);//使用窗口管理器
    editor->setWindowTitle("编辑");
    //editor->setAttribute(Qt::WA_DeleteOnClose);
    editor->resize(200,350);
    editor->move(QApplication::desktop()->width()/2-editor->width()/2,QApplication::desktop()->height()/2-editor->height()/2);
    connect(editor,SIGNAL(setfinished(LayerItem,QPixmap)),this,SLOT(editfinished(LayerItem)));
    connect(editor,SIGNAL(editcancel()),this,SLOT(editcancel()));
    editor->show();
}

void LayerTableView::editfinished(const LayerItem &i)
{
    if(doing == EditItem || doing == EditbyNote)
    {
        desktopNote *note;
         QModelIndex index;
        int row = model->findbbyid(i.id);
        index = model->index(row,1);
        qDebug()<<index.row()<<index.column();
        QVariant data;
        data.setValue(i);
        model->setData(index,data,Qt::EditRole);
        index = model->index(row,0);//设置isenable
        data.setValue(i.isenable);
        model->setData(index,data,Qt::CheckStateRole);
        if(i.isnote^hasitem(i.id))//异或，有变无，无变有
        {
            qDebug()<<"note has change";
            if(i.isnote){
                //无变有
                qDebug()<<"none->has";
                if(i.color.indexOf("png")!=-1)
                    note = new desktopNote(i,true,this);
                else
                    note = new desktopNote(i,false,this);
                connect(note,SIGNAL(finished(LayerItem)),this,SLOT(noteedited(LayerItem)));
                connect(note,SIGNAL(addnote()),this,SLOT(addanote()));
                connect(note,SIGNAL(reboot2alpha(LayerItem&)),this,SLOT(noteneedreboot(LayerItem&)));
                connect(note,SIGNAL(notedel(QString)),this,SLOT(delnote(QString)));
                note->setdata(i);
                note->initnote();
                qDebug()<<i.id;
                notelist.insert(i.id,note);
            }
            else {//有变无
                qDebug()<<"has->none";
                note = notelist.value(i.id);
                note->close();
                note = nullptr;
                notelist.remove(i.id);
            }

        }
        else if(i.isnote && doing == EditItem) {
            //通过便签设置内容
            qDebug()<<"edititem by note";
            note = notelist.value(i.id);
            note->setdata(i);//设置新数据
            note->initnote();//刷新便签
        }


    }
    else if(doing == AddItem||doing == AddItemNoUpdateList)
    {
        LayerItem item = i;
        if(doing == AddItemNoUpdateList){
            model->addItem(item,false);//这里是读取配置用的，已经有id
        }
        else {
            item.id = QDateTime().currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            qDebug()<<"id:"<<item.id;
            model->addItem(item,true);
            model->refreshModel();
            this->resizeRowsToContents();
        }
        if(item.isnote)
        {
            desktopNote *note;
            if(item.color.indexOf("png")!=-1)
                note = new desktopNote(item,true,this);
            else
                note = new desktopNote(item,false,this);
            connect(note,SIGNAL(finished(LayerItem)),this,SLOT(noteedited(LayerItem)));
            connect(note,SIGNAL(addnote()),this,SLOT(addanote()));
            connect(note,SIGNAL(reboot2alpha(LayerItem&)),this,SLOT(noteneedreboot(LayerItem&)));
            connect(note,SIGNAL(notedel(QString)),this,SLOT(delnote(QString)));

            //note->setdata(item);
            notelist.insert(item.id,note);
        }
    }

    model->update_taskslist();
    doing = Nothing;
    if(editor!=nullptr) editor->close();
    editor = nullptr;
    qDebug()<<"editfinished";
}

bool LayerTableView::hasitem(QString id)
{
    for(int i=0;i<notelist.count();i++)
    {
        if(notelist.contains(id)) return true;
    }
    return false;
}

void LayerTableView::noteedited(const LayerItem& item){
    LayerItem i = item;
    doing = EditbyNote;
    editfinished(i);
}

void LayerTableView::addanote()
{
    doing = AddItem;
    LayerItem item;
    item.color="#ffffff";
    item.isenable = true;
    item.isnote = true;
    item.type = 0;
    item.pre = "1";
    item.note="";
    editfinished(item);
}

void LayerTableView::noteneedreboot(LayerItem &i){
    desktopNote *note;
    note = notelist.value(i.id);
    note->close();
    note = nullptr;
    notelist.remove(i.id);
    editfinished(i);

}

void LayerTableView::delnote(QString id)
{
    int row = model->findbbyid(id);
    this->setCurrentIndex(model->index(row,1));
    deleteLayer();
}

void LayerTableView::editcancel()
{
    editor->close();
    editor = nullptr;
}

void LayerTableView::changenote()
{
    QModelIndex index = this->currentIndex();
    index = model->index(index.row(),1);
    LayerItem item = model->data(index,Qt::EditRole).value<LayerItem>();
    item.isnote = !item.isnote;//取反
    doing = EditItem;
    editfinished(item);
}

void LayerTableView::savetheme()
{
    QString cfg = cfgpath+"/Theme";
    QSettings settings(cfg,QSettings::NativeFormat);
    settings.setValue("selectedcolor", selectedcolor);
    settings.setValue("textcolor", textcolor);
    settings.setValue("timecolor",timecolor);
    settings.setValue("datecolor",datecolor);
    settings.setValue("datebg",datebg);
    settings.setValue("tipcolor", tipcolor);
    settings.setValue("tipbg",tipbg);
    settings.setValue("onpic",onpic);
    settings.setValue("offpic",offpic);
    qDebug()<<"LayerTableView::savetheme()";
}

void LayerTableView::readtheme()
{
    QString cfg = cfgpath+"/Theme";
    QSettings settings(cfg,QSettings::NativeFormat);
    selectedcolor= settings.value("selectedcolor", "").toString();
    textcolor= settings.value("textcolor", "").toString();
    timecolor= settings.value("timecolor","").toString();
    datecolor= settings.value("datecolor","").toString();
    datebg= settings.value("datebg","").toString();
    tipcolor= settings.value("tipcolor", "").toString();
    tipbg= settings.value("tipbg","").toString();
    onpic= settings.value("onpic",false).toBool();
    offpic= settings.value("offpic",false).toBool();
    qDebug()<<"LayerTableView::readtheme()";
}
