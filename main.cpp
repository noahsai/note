
/*【注意】
 * 该项目由网上资源修改而来。
 * 原代码出处：http://blog.csdn.net/silangquan/article/details/41440467
 * 衷心感谢代码原作者的无私奉献，将qt的delegate，model，viewd的实际使用方法放到网上，
 * 使我能找到一篇中文的，有实际意义的，内容全面的教程。
 * 谢谢！
 * 如有侵权，请在github上联系我。
*/

#include <QApplication>
#include<layertableview.h>
#include<QDesktopWidget>
#include<QDir>
#include<QStandardPaths>
//-------------------
QString cfgpath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/.config/notewei";

int main(int argc, char *argv[])
{


//其他系统不能之打开一个实例，主要是我自己写的‘只运行一个实例’的功能有问题，所以该功能直接用dtk的了
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    QDir().mkpath(cfgpath);
    LayerTableView w;
    w.setStyleSheet("QTableView {\
                selection-background-color :  qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(125, 214, 221, 255), stop:0.505618 rgba(128, 199, 223, 255), stop:1 rgba(108, 201, 230, 255));\
                             gridline-color:rgb(230, 230, 230) ;}");
    w.setMinimumSize(200,250);

    w.setWindowTitle("NoteWei");
    w.setWindowIcon(QIcon(":/wei2.png"));
    return a.exec();


    return 0;


}


