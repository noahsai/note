#include "notifyset.h"
#include "ui_notifyset.h"

notifyset::notifyset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::notifyset)
{
    ui->setupUi(this);
    //setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    selectedcolor_t = selectedcolor;
    textcolor_t = textcolor;
    timecolor_t = timecolor;
    datecolor_t= datecolor;
    datebg_t = datebg;
    tipcolor_t = tipcolor;
    tipbg_t = tipbg;
    onpic_t = onpic;
    offpic_t = offpic;
    init();
}

notifyset::~notifyset()
{
    delete ui;
}

void notifyset::on_yes_clicked()
{
    int t=0;
    QString time = ui->time->text();
    QString music = ui->music->text();
    QString icon = ui->icon->text();

    t = time.toInt();
    t*=1000;
    if(t==0) t=5000;
    emit ok(t,music,icon);
}

void notifyset::on_getmusic_clicked()
{
    QString home = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
    QString file =  QFileDialog::getOpenFileName(this,"选择音乐",home,tr("所有 (*.*)"));
    if(file.isNull()) return;
    else ui->music->setText(file);
}

void notifyset::on_geticon_clicked()
{
    QString home = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
    QString file =  QFileDialog::getOpenFileName(this,"选择图片",home,tr("所有 (*.*)"));
    if(file.isNull()) return;
    else ui->icon->setText(file);
}

void notifyset::setinit(int t,QString m,QString i)
{
    t = t/1000;
    ui->time->setText(QString().setNum(t));
    ui->music->setText(m);
    ui->icon->setText(i);
}

void notifyset::on_no_clicked()
{
    selectedcolor=selectedcolor_t;
    textcolor=textcolor_t;
    timecolor=timecolor_t;
    datecolor=datecolor_t;
    datebg=datebg_t;
    tipcolor=tipcolor_t;
    tipbg=tipbg_t;
    onpic=onpic_t;
    offpic=offpic_t;
    emit selectcolorset();
    close();
}

void notifyset::on_pushButton_clicked()
{
    ui->time->setText(QString().setNum(10));
    ui->music->setText(":/wei4.mp3");
    ui->icon->setText(":/wei.png");

    selectedcolor="";
    textcolor="";
    timecolor="";
    datecolor="";
    datebg="";
    tipcolor="";
    tipbg="";
    onpic=false;
    offpic=false;
    emit selectcolorset();
    init();
}


void notifyset::on_selcolor_button_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if(!color.isValid()) return;
    QString t = color.name();
    ui->selcolor_text->setText(t);;
    selectedcolor = t;
    t = "#selcolor_text{color:"+t+";}";
    ui->selcolor_text->setStyleSheet(t);
    emit selectcolorset();
    emit freshtheme();
}

void notifyset::on_textcolor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if(!color.isValid()) return;
    QString t = color.name();
    textcolor = t;
    t = "#textcolor{color:"+t+";}";
    ui->textcolor->setStyleSheet(t);
    emit freshtheme();
}

void notifyset::on_timecolor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if(!color.isValid()) return;
    QString t = color.name();
    timecolor = t;
    t = "#timecolor{color:"+t+";}";
    ui->timecolor->setStyleSheet(t);
    emit freshtheme();
}

void notifyset::on_datecolor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if(!color.isValid()) return;
    QString t = color.name();
    datecolor = t;
    t = "#datecolor{color:"+t+";}";
    ui->datecolor->setStyleSheet(t);
    emit freshtheme();
}

void notifyset::on_datebg_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if(!color.isValid()) return;
    QString t = color.name();
    datebg = t;
    t = "#datebg{color:"+t+";}";
    ui->datebg->setStyleSheet(t);
    emit freshtheme();
}

void notifyset::on_tipcolor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if(!color.isValid()) return;
    QString t = color.name();
    tipcolor = t;
    t = "#tipcolor{color:"+t+";}";
    ui->tipcolor->setStyleSheet(t);
    emit freshtheme();
}

void notifyset::on_tipbg_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if(!color.isValid()) return;
    QString t = color.name();
    tipbg = t;
    t = "#tipbg{color:"+t+";}";
    ui->tipbg->setStyleSheet(t);
    emit freshtheme();
}

void notifyset::on_onpic_clicked()
{
    QString home = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
    QString file =  QFileDialog::getOpenFileName(this,"选择图片",home,tr("图片 (*.png)"));
    if(file.isNull()) return;
    QFile f(cfgpath + "/on.png");
    f.remove();
    f.setFileName(file);
    f.close();
    if(f.copy(cfgpath+"/on.png")){
        ui->onpic->setIcon(QIcon(cfgpath + "/on.png"));
        onpic = true;
    }
    emit freshtheme();
}

void notifyset::on_offpic_clicked()
{
    QString home = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
    QString file =  QFileDialog::getOpenFileName(this,"选择图片",home,tr("图片 (*.png)"));
    if(file.isNull()) return;
    QFile f(cfgpath + "/off.png");
    f.remove();
    f.setFileName(file);
    f.close();
    if(f.copy(cfgpath+"/off.png")){
        ui->offpic->setIcon(QIcon(cfgpath + "/off.png"));
        offpic = true;
    }
    emit freshtheme();
}

void notifyset::init()
{
    QString t = selectedcolor;
    ui->selcolor_text->setText(t);;
    t = "#selcolor_text{color:"+t+";}";
    ui->selcolor_text->setStyleSheet(t);

    t = textcolor;
    t = "#textcolor{color:"+t+";}";
    ui->textcolor->setStyleSheet(t);

    t = timecolor;
    t = "#timecolor{color:"+t+";}";
    ui->timecolor->setStyleSheet(t);

    t = datecolor;
    t = "#datecolor{color:"+t+";}";
    ui->datecolor->setStyleSheet(t);

    t = datebg;
    t = "#datebg{color:"+t+";}";
    ui->datebg->setStyleSheet(t);

    t = tipcolor;
    t = "#tipcolor{color:"+t+";}";
    ui->tipcolor->setStyleSheet(t);

    t = tipbg;
    t = "#tipbg{color:"+t+";}";
    ui->tipbg->setStyleSheet(t);

    if(onpic)     ui->onpic->setIcon(QIcon(cfgpath + "/on.png"));
    else    ui->onpic->setIcon(QIcon(":/on.png"));
    if(offpic)    ui->onpic->setIcon(QIcon(cfgpath + "/off.png"));
    else    ui->offpic->setIcon(QIcon(":/off.png"));

}
