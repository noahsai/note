#include "notifyset.h"
#include "ui_notifyset.h"

notifyset::notifyset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::notifyset)
{
    ui->setupUi(this);
}

notifyset::~notifyset()
{
    delete ui;
}

void notifyset::on_yes_clicked()
{
    int t=60000;
    QString time = ui->time->text();
    QString music = ui->music->text();
    QString icon = ui->icon->text();
    if(!time.isEmpty() && time.toInt()==0)
    {
        QMessageBox::warning(this,"时间输入有误","输入为0或不是数字！\n请正确输入秒数，或者不填写。",QMessageBox::Ok	);
        return;
    }
    t = time.toInt();
    t*=1000;
    emit ok(t,music,icon);
}

void notifyset::on_getmusic_clicked()
{
    QString home = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString file =  QFileDialog::getOpenFileName(this,"选择音乐",home,tr("所有 (*.*)"));
    if(file.isNull()) return;
    else ui->music->setText(file);
}

void notifyset::on_geticon_clicked()
{
    QString home = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString file =  QFileDialog::getOpenFileName(this,"选择图片",home,tr("所有 (*.*)"));
    if(file.isNull()) return;
    else ui->icon->setText(file);
}

