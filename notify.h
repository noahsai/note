#ifndef NOTIFY_H
#define NOTIFY_H

#include <QWidget>
#include<QTimer>
#include<QMediaPlayer>
#include<QDesktopWidget>
#include<QMouseEvent>
#include <QSettings>

namespace Ui {
class Notify;
}

class Notify : public QWidget
{
    Q_OBJECT

public:
    explicit Notify(QWidget *parent = 0);
    ~Notify();
    void init(int t, QString& i , QString& m);
    void message(QString &);
    void reset();

private slots:
    void timeout();
    void repeat(QMediaPlayer::MediaStatus);
private:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void saveset();
    void readset();

    Ui::Notify *ui;
    QStringList *list;
    QTimer* timer;

    bool mousepressed;
    QPoint oldpos;

    int  time;
    QString icon , music;
    QMediaPlayer* player;
    notifyset *set;
    Notify *notify;
};

#endif // NOTIFY_H
