#ifndef NOTIFYSET_H
#define NOTIFYSET_H

#include <QDialog>
#include<QFileDialog>
#include<QStandardPaths>
#include<QMessageBox>
#include<QColorDialog>

extern QString cfgpath;//全局变量

//==Theme==
extern QString selectedcolor;
extern QString textcolor;
extern QString timecolor;
extern QString datecolor;
extern QString datebg;
extern QString tipcolor;
extern QString tipbg;
extern bool onpic;
extern bool offpic;
//=============


namespace Ui {
class notifyset;
}

class notifyset : public QDialog
{
    Q_OBJECT

public:
    explicit notifyset(QWidget *parent = 0);
    ~notifyset();
    void setinit(int ,QString ,QString );

private slots:
    void on_yes_clicked();

    void on_getmusic_clicked();

    void on_geticon_clicked();

    void on_no_clicked();

    void on_pushButton_clicked();


    void on_selcolor_button_clicked();

    void on_textcolor_clicked();

    void on_timecolor_clicked();

    void on_datecolor_clicked();

    void on_datebg_clicked();

    void on_tipcolor_clicked();

    void on_tipbg_clicked();

    void on_onpic_clicked();

    void on_offpic_clicked();


signals:
    void ok(int ,QString ,QString );
    void selectcolorset();
    void freshtheme();

private:
    Ui::notifyset *ui;
    QString selectedcolor_t;
    QString textcolor_t;
    QString timecolor_t;
    QString datecolor_t;
    QString datebg_t;
    QString tipcolor_t;
    QString tipbg_t;
    bool onpic_t;
    bool offpic_t;
    void init();
};

#endif // NOTIFYSET_H
