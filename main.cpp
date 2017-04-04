
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
//deepin下编译用，非dtk环境下注释掉即可
#include<QObject>
#include <DApplication>
#include <QProcess>
#include <QString>
DWIDGET_USE_NAMESPACE
//-------------------

QString cfgpath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/.config/notewei";

int main(int argc, char *argv[])
{

    QProcess pro;
    pro.start("uname -a");
    if(pro.waitForFinished(3000))
    {
        QString system = pro.readAllStandardOutput();
        if(system.indexOf("eepin")!=-1)
        {
            qDebug()<<"deepin下支持只打开一个此软件";
            DApplication app(argc, argv);
            QDir().mkpath(cfgpath);
            app.setQuitOnLastWindowClosed(false);
            //不知为和，这个程序需要这样设置才不会在关闭便签时退出整个程序。
            //可能是因为便签的flag不是窗口，所以隐藏主程序时，关闭便签会触发该函数。
            LayerTableView w;
            if (app.setSingleInstance("noahsai-notewei")) {
                QObject::connect(&app,&DApplication::newInstanceStarted,&w,&LayerTableView::show);

                w.setStyleSheet("QTableView {\
                            selection-background-color :  qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(125, 214, 221, 255), stop:0.505618 rgba(128, 199, 223, 255), stop:1 rgba(108, 201, 230, 255));\
                                         gridline-color:rgb(230, 230, 230) ;\}");
                w.setMinimumSize(200,250);

                w.setWindowTitle("NoteWei");
                w.setWindowIcon(QIcon(":/wei2.png"));
                return app.exec();
            }
            return 0;
        }
        else {
            //其他系统不能之打开一个实例，主要是我自己写的‘只运行一个实例’的功能有问题，所以该功能直接用dtk的了
                QApplication a(argc, argv);
                a.setQuitOnLastWindowClosed(false);
                QDir().mkpath(cfgpath);
                LayerTableView w;
                w.setStyleSheet("QTableView {\
                            selection-background-color :  qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(125, 214, 221, 255), stop:0.505618 rgba(128, 199, 223, 255), stop:1 rgba(108, 201, 230, 255));\
                                         gridline-color:rgb(230, 230, 230) ;\}");
                w.setMinimumSize(200,250);

                w.setWindowTitle("NoteWei");
                w.setWindowIcon(QIcon(":/wei2.png"));
                return a.exec();
        }
    }
    return 0;
// w.move(QApplication::desktop()->width()/2-w.width()/2,QApplication::desktop()->height()/2-w.height()/2);
// w.show();//在view里控制了
//  desktopNote n(false);
//  n.show();
//  return a.exec();

}


