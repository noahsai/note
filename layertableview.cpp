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
  //  timer.setSingleShot(false);
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
    QAction *pAddGroupAct = new QAction(tr("添加"), pMenu);
    connect(pAddGroupAct ,SIGNAL(triggered(bool)),this,SLOT(addNewLayer()) );
    pMenu->addAction(pAddGroupAct);
    pAddGroupAct = new QAction(tr("删除"), pMenu);
    connect(pAddGroupAct ,SIGNAL(triggered(bool)),this,SLOT(deleteLayer()) );
    pMenu->addAction(pAddGroupAct);
    pMenu->popup(mapToGlobal(event->pos()));

}

void LayerTableView::addNewLayer()
{
    model->addItem();
    model->refreshModel();
    this->resizeRowsToContents();
    //空白的就不保存了。
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
//        else if (index.column() == 1)
//        {
//            model->setSelecttedRow(index.row());
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



void LayerTableView::timeout()
{
    timer.stop();
    QList<todayitem> todaylist = model->get_tarkslist();
    for(int i=0;i<todaylist.size();i++)
    {
      //  qDebug()<<todaylist[i].time;
        QTime now = QTime::currentTime();
        if(todaylist.at(i).time.toString("h:m:s")==now.toString("h:m:s"))
        {
//            qDebug()<<i<<"bling!!!!!!"<<todaylist.at(i).time.toString("h:m:s");
            qDebug()<<"bling!!!!!!";
            system("notify-send \"hi!note!\"");
            if(todaylist[i].onetime) itemClicked(todaylist[i].index);
        }
    }
    timer.start(1000);
}
