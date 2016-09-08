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
    QString a;
    QImage m(layerSize, QImage::Format_RGB32);
    model->addItem();
    //model->addItem(QString(), QImage("images\\sample.jpg"), true);
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
}

void LayerTableView::setLayerSize(QSize s)
{
    layerSize = s;
}
