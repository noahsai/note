#include <QApplication>
#include<layertableview.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // Note w;
    LayerTableView w;
    w.setMinimumSize(400,450);
    w.show();

    return a.exec();
}
