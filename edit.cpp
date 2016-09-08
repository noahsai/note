#include "edit.h"
#include "ui_edit.h"

edit::edit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::edit)
{
    ui->setupUi(this);
}

edit::~edit()
{
    delete ui;
}

void edit::on_note_textChanged(const QString &arg1)
{
    item.note=arg1;
}

void edit::on_onetime_clicked(bool checked)
{

    item.onetime = checked;
}

void edit::on_type_currentIndexChanged(int index)
{
    item.type = index;
    switch(index){
    case 0 :
        sethide_show(true,true,false);
        ui->label->setText("日期");
        ui->label_2->setText("时间");
        break;
    case 1:
        sethide_show(false,false,true);
        break;
    case 2:
        sethide_show(true,true,true);
        ui->label->setText("起始日期");
        ui->label_2->setText("提醒时间");
        break;
    case 3:
        sethide_show(false,true,true);
        ui->label_2->setText("起始时间");
        break;

    }

}


void edit::on_date_textChanged(const QString &arg1)
{
    item.date = arg1;
}

void edit::on_time_textChanged(const QString &arg1)
{
    item.time = arg1;
}

void edit::on_pre_textChanged(const QString &arg1)
{
    item.pre = arg1;
}


void edit::setdata(LayerItem &value)
{
    item = value;
    ui->date ->setText( value.date);
    ui->note ->setText(value .note);
    ui->onetime->setChecked(value.onetime);
    ui->pre ->setText(value.pre);
    ui->time ->setText( value.time);
    ui->type ->setCurrentIndex( value.type);
}

void edit::on_onetime_clicked( )
{

}

void edit::sethide_show(bool d, bool t, bool p)
{
    ui->date->setVisible(d);
    ui->label->setVisible(d);
    ui->time->setVisible(t);
    ui->label_2->setVisible(t);
    ui->pre->setVisible(p);
    ui->label_3->setVisible(p);
}
