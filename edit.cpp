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
        sethide_show(false,true,true);
        ui->label_2->setText("提醒时间");
        break;
    case 2:
        sethide_show(true,true,true);
        ui->label->setText("起始日");
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
    //item.date = arg1;
}

void edit::on_time_textChanged(const QString &arg1)
{
   // item.time = arg1;
}

void edit::on_pre_textChanged(const QString &arg1)
{
   // item.pre = arg1;
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
    if(value.type==-1) ui->type->setCurrentIndex(0);
    if(value.time.isEmpty()) {
        item.time=QTime::currentTime().toString("h:m:s");
        item.onetime = true;
    }
    if(value.date.isEmpty())
    {
        QString date = QDate::currentDate().toString("yyyy.M.d");
        item.date = date;
    }

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

void edit::on_date_editingFinished()
{
    QString text = ui->date->text();
    qDebug()<<"date input"<<text;
    if(text.isEmpty()) return;
    QRegularExpression reg;
    reg.setPattern("[:-/,]");
    text = text.replace(reg,".");
    reg.setPattern("(20\\d\\d\\.){0,1}(\\d{1,2}\\.){0,1}\\d{1,2}$");//2016.3.12
    int result = text.indexOf(reg);
    if(result!=0)
        QMessageBox::warning(NULL, "输入错误", "格式: 20xx.x.x 或 20xx-x-x 或 20xx/x/x \n注意：月，日超出范围不会提示。请正确填写。"
                                           "","返回",0);
    else item.date = text;

}

void edit::on_time_editingFinished()
{
    QString text = ui->time->text();
    if(text.isEmpty()) return;
    QRegularExpression reg;
    reg.setPattern("[.-/]");
    text = text.replace(reg,":");
    reg.setPattern("\\d{1,2}:\\d{1,2}:{0,1}\\d{1,2}$");//01:21
    if(text.indexOf(reg)!=0)   {
        QMessageBox::warning(NULL, "输入错误", "格式: 12:34  \n注意：时，分超出范围不会提示。请正确填写。","返回",0);
    }
    else {
        item.time = text;
    }
}

void edit::on_pre_editingFinished()
{
    QString text = ui->pre->text();
    int type =ui->type->currentIndex();
    if(type==1)
    {
        text = text.remove(QRegularExpression("[890a-zA-Z]"));
        text = text.replace(QRegularExpression("[，.。/；]"),",");
    }
    else
    {
        text = QRegularExpression("\\d+").match(text).captured();
    }
    item.pre = text;
}
