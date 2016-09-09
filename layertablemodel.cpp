#include "layertablemodel.h"

LayerTableModel::LayerTableModel(QObject *parent)
: QAbstractTableModel(parent)
{
    selectedRow = 0;

    layerList.reserve(3);
//    for (int i = 0; i < 3; i++)
//    {
//        QString a;
//        addItem();
//    }
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
        savelist();
        return true;
    }
    if (role == Qt::EditRole && index.column() == 1)
    {
        LayerItem va = val.value<LayerItem>();
        layerList.replace(index.row(),va);
        emit(dataChanged(index, index));
        savelist();
        return true;
    }
    return false;;
}

void LayerTableModel::changeLayerVisibility(const QModelIndex& index)
{
    if (index.isValid()&&index.column() == 0)
    {
        setData(index, !(layerList.at(index.row()).isenable), Qt::CheckStateRole);
    }
}

void LayerTableModel::deleteItem(int index)
{
    qDebug()<<"remove:"<<index;
    QList<LayerItem>::iterator it = layerList.begin();
    layerList.erase(it + index);
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


void LayerTableModel::addItem(LayerItem& add)
{
    LayerItem item = add;
    layerList.append(item);
    //this->insertRow()
    //emit(dataChanged(index, index));
    qDebug()<<layerList.size();
    savelist();
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

void LayerTableModel::setSelecttedRow(int row)
{
    selectedRow = row;
}

int LayerTableModel::getSelecttedRow() const
{
    return selectedRow;
}

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


QList<LayerItem> LayerTableModel:: getlist(){
    return layerList;
}
