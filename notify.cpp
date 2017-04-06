#include "notify.h"
#include "ui_notify.h"
Notify::Notify(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Notify)
{
    ui->setupUi(this);
    readset();
    setAttribute(Qt::WA_TranslucentBackground );//背景透明
    setWindowFlags(Qt::ToolTip);
    connect(ui->pushButton,SIGNAL(clicked(bool)), this,SLOT(stop()));
    oldpos.setX(0);
    oldpos.setY(0);
    mousemoving = false;
    list = new QStringList;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
    player = new QMediaPlayer(this);
    connect(player , SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(repeat(QMediaPlayer::MediaStatus)));
    timer->setSingleShot(true);
}

Notify::~Notify()
{
    delete ui;
}

void Notify::init(int t, QString &m, QString &i)
{
    time = t;
    icon =i;
    if(i.indexOf(":/wei.png")==0)
    {
        ui->icon->setStyleSheet(
                        "QLabel\
                        {\
                        border-top:1px solid     rgb(131, 131, 131);\
                        border-left:1px solid      rgb(131, 131, 131);\
                        border-bottom:1px solid     rgb(131, 131, 131);\
                        }");
        ui->note->setStyleSheet(
                        "QLabel\
                        {\
                        background-color:white;\
                        border-top:1px solid     rgb(131, 131, 131);;\
                        border-right:1px solid      rgb(131, 131, 131);;\
                        border-bottom:1px solid     rgb(131, 131, 131);;\
                        }");
    }
    else
    {
        ui->note->setStyleSheet(
                        "QLabel\
                        {\
                        background-color:white;\
                        border-top:1px solid      rgb(131, 131, 131);;\
                        border-left:1px solid      rgb(131, 131, 131);;\
                        border-right:1px solid      rgb(131, 131, 131);;\
                        border-bottom:1px solid     rgb(131, 131, 131);;\
                        }");
        ui->icon->setStyleSheet("");
    }
    QPixmap pix(i);
    pix = pix.scaled(120,150,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->icon->setPixmap(pix);
    music = m;
    if(music.indexOf(":/wei4.mp3")==0) music = QApplication::applicationDirPath()+"/wei4.mp3";
    qDebug()<<"notifymusic:"<<music;
    player->setVolume(100);
}

void  Notify::message(QString& mes)
{
    qDebug()<<"get a message";
     list->append(mes);
     if( list -> count() ==1 )
     {
         ui->note->setText(mes);
         player->setMedia(QUrl::fromLocalFile(music));
         player->play();
        timer->start( time );
     }
}

void Notify::timeout()
{
    list->removeFirst();
    if( ! list->isEmpty()) {
        ui->note->setText(list->first());
        timer->start(time);
    }
    else {
        stop();
    }
}

void Notify::stop()
{
    reset();
    hide();
}


void Notify::repeat(QMediaPlayer::MediaStatus s)
{
    if (s == QMediaPlayer::EndOfMedia) {

        player->play();
    }
}

void Notify::reset()
{
    list->clear();
    player->stop();
    timer->stop();
    list->clear();
}

//=====================================================================
void Notify::mousePressEvent(QMouseEvent* event)
{
    if(event->button()==Qt::LeftButton && !mousemoving)
    {
        oldpos=event->globalPos()-this->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void Notify::mouseMoveEvent(QMouseEvent * event){
    if(event->buttons()==Qt::LeftButton)
    {
        mousemoving = true;
        move(event->globalPos()-oldpos);//貌似linux要这样
        event->accept();
    }
}

void Notify::mouseReleaseEvent(QMouseEvent * event){
    if(event->button()==Qt::LeftButton && mousemoving)
    {
        int x=this->x();
        int y=this->y();
//        qDebug()<<x<<endl<<y;

        if(this->pos().x()<0) x=0;
        else if(QApplication::desktop()->width()-x<this->width()){
            x=QApplication::desktop()->width()-this->width();
        }

        if(this->pos().y()<0) y=0;
        else if(QApplication::desktop()->height()-y<this->height())
        {
            y=QApplication::desktop()->height()-this->height();
        }
//        qDebug()<<x<<endl<<y;
        move(x,y);
        mousemoving=false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        saveset();
    }
    else if(event->button()==Qt::LeftButton && (!mousemoving)) {
        timer->stop();
        timeout();
    }
    update();

}


void Notify::saveset(){
    QSettings settings("ShengSoft", "Notify");
   // settings.setValue("size", QSize(370, 150));//因为没得调大小，所以不要记录大小了
    settings.setValue("pos", pos());
}


void Notify::readset(){
    QSettings settings("ShengSoft", "Notify");
//    resize(settings.value("size", QSize(370, 150)).toSize());//因为没得调大小，所以不要记录大小了
    int x=QApplication::desktop()->width()-420;
    QPoint point;
    point=settings.value("pos", QPoint(x, 50)).toPoint();
    if(point.x()<0||point.x()>QApplication::desktop()->width()-20) point.setX(x);
    if(point.y()<0||point.y()>QApplication::desktop()->height()+this->height()-30) point.setY(50);
    move(point);
}
