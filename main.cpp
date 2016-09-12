#include <QApplication>
#include<layertableview.h>
#include<QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // Note w;
    LayerTableView w;
    w.setStyleSheet("QTableView {\
                selection-background-color :  qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(125, 214, 221, 255), stop:0.505618 rgba(128, 199, 223, 255), stop:1 rgba(108, 201, 230, 255));\
                             gridline-color:rgb(230, 230, 230) \
}");
    w.setMinimumSize(200,250);
  //  w.resize(360,450);

    w.setWindowTitle("提醒");
    w.setWindowIcon(QIcon(":/wei2.png"));
   // w.move(QApplication::desktop()->width()/2-w.width()/2,QApplication::desktop()->height()/2-w.height()/2);
    w.show();

    return a.exec();
}


