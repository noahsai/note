#include "notifyset.h"
#include "ui_notifyset.h"

notifyset::notifyset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::notifyset)
{
    ui->setupUi(this);
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
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
    close();
}

void notifyset::on_pushButton_clicked()
{
    ui->time->setText(QString().setNum(5));
    ui->music->setText(":/wei4.mp3");
    ui->icon->setText(":/wei.png");
}
