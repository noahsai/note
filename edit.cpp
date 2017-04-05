#include "edit.h"
#include "ui_edit.h"

editnote::editnote(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::edit)
{
    ui->setupUi(this);
    this->setMinimumWidth(150);//最新宽度150
    setAttribute(Qt::WA_DeleteOnClose);
    this->setSizePolicy(QSizePolicy::Ignored ,QSizePolicy::Preferred );
    ui->size->setValue(12);//默认值，最大最小在ui里设置了
    //setbgcolor("ffffff");
    inititem();//初始化一个item
    QFont fo;
    fo.setPointSize(12);
    ui->note->document()->setDefaultFont(fo);
    ui->color->setStyleSheet("QPushButton{color:#000000;}");
}

editnote::~editnote()
{
    delete ui;
}

void editnote::inititem()
{
    QString date = QDate::currentDate().toString("yyyy.M.d");
    item.date = date;
    //默认当天
    item.time=QTime::currentTime().toString("hh:mm:ss");
    //默认现在时间
    ui->onetime->setChecked(false);//默认周期性
    item.onetime = false;
    ui->enable->setChecked(true);
    item.isenable = true ;//默认开启
    ui->checkBox->setChecked(true);
    item.isnote = false;//默认不是便签
    ui->type->setCurrentIndex(0);
    on_type_currentIndexChanged(0);
    item.type = 0;//默认定期事件
    item.isenable = true;
    item.isnote = true;
}


void editnote::on_note_textChanged()
{
    QString text;
    text = ui->note->toPlainText().remove("\n").remove(" ");
    item.note= text;
    item.html=ui->note->toHtml();
//    qDebug()<<ui->note->toPlainText();
//    qDebug()<<ui->note->toHtml();

}

void editnote::on_onetime_clicked(bool checked)
{

    item.onetime = checked;
}

void editnote::on_type_currentIndexChanged(int index)
{
    item.type = index;
    on_time_editingFinished();
    on_date_editingFinished();
    on_pre_editingFinished();
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


void editnote::on_date_textChanged(const QString &arg1)
{
    //item.date = arg1;
}

void editnote::on_time_textChanged(const QString &arg1)
{
   // item.time = arg1;
}

void editnote::on_pre_textChanged(const QString &arg1)
{
   // item.pre = arg1;
}


void editnote::setdata(LayerItem &value)
{
    ui->note->clear();
    item = value;
    ui->date ->setText( value.date);
    ui->onetime->setChecked(value.onetime);
    ui->pre ->setText(value.pre);
    ui->time ->setText( value.time);
    ui->type ->setCurrentIndex( value.type);
    ui->note->setHtml(value.html);
    ui->checkBox->setChecked(value.isnote);//便签
    ui->enable->setChecked(value.isenable);
    setbgcolor(item.color,true);
    if(value.type==-1) ui->type->setCurrentIndex(0);
    if(value.date.isEmpty())
    {
        QString date = QDate::currentDate().toString("yyyy.M.d");
        item.date = date;
    }
    if(value.time.isEmpty()) {
        item.time=QTime::currentTime().toString("hh:mm:ss");
    }
    qDebug()<<this->geometry()<<ui->note->size();

}

void editnote::on_onetime_clicked( )
{
}

void editnote::sethide_show(bool d, bool t, bool p)
{
    ui->date->setVisible(d);
    ui->label->setVisible(d);
    ui->time->setVisible(t);
    ui->label_2->setVisible(t);
    ui->pre->setVisible(p);
    ui->label_3->setVisible(p);
}

void editnote::on_date_editingFinished()
{
    QString text = ui->date->text();
    qDebug()<<"date input"<<text;
    if(text.isEmpty()){
        item.date=QDate::currentDate().toString("yyyy.M.d");//空的设置当前日期
        return;
    }
    QRegularExpression reg;
    reg.setPattern("[:-/,]");
    text = text.replace(reg,".");
    reg.setPattern("(\\d{4}\\.){0,1}(\\d{1,2}\\.){0,1}\\d{1,2}$");//2016.3.12
    if((text.indexOf(reg)!=0)||(text.indexOf(QRegularExpression("\\d{4}\\.\\d{1,2}$"))==0))
        QMessageBox::warning(NULL, "输入错误", "请正确填写。\n格式: 20xx.x.x 或 xx-xx \n注意：月，日超出范围不会提示。","返回",0);
    else {
        item.date = text;
        ui->date->setText(text);
    }

}

void editnote::on_time_editingFinished()
{
    QString text = ui->time->text();
    if(text.isEmpty()) {
        item.time=QTime::currentTime().toString("hh:mm:ss");//空的设置当前时间
        return;
    }
    QRegularExpression reg;

    QTime time;
    if(item.type == 3){
        reg.setPattern(":.+:");
        if(text.indexOf(reg)==-1){
            text = text+":00";
        }
        time = time.fromString(text,"h:m:s");
        text = time.toString("hh:mm:ss");

    }
    else {
        if(text.indexOf(QRegularExpression(":.+:"))!=-1){
            reg.setPattern(":\\d{0,2}$");
            text = text.remove(reg);
        }
        time = time.fromString(text,"h:m");
        text = time.toString("hh:mm");
    }
    if(time.isNull()){
        QMessageBox::warning(NULL, "输入错误", "请正确填写。\n\
                                    格式: 12:34 或 12:3:45 \n\
                                    注意：空白为当前时间","返回",0);

    }
    else {
        item.time = text;
        ui->time->setText(text);
    }
    //qDebug()<<time<<text;

}

void editnote::on_pre_editingFinished()
{
    QString text = ui->pre->text();
    int type =ui->type->currentIndex();
    if(type==1)
    {
        //生成星期列表
        QString days,one;
        for(int i=1;i<8;i++)
        {
            one.setNum(i);
            if(text.indexOf(one)!=-1)
            {
                days = days + one;
                if(i!=7) days +=",";
            }
        }
        text = days;
    }
    else
    {
        text = text.remove(QRegularExpression("[^0-9]"));//去除非数字
        text = QRegularExpression("\\d*").match(text).captured();
    }
    item.pre = text;
    ui->pre->setText(text);
}

void editnote::on_pushButton_clicked()
{
    on_date_editingFinished();
    on_time_editingFinished();
    //qDebug()<<"warp"<<ui->note->document()->defaultTextOption().wrapMode();
    QPixmap pix(ui->note->width(),ui->note->height());
    if(item.color.indexOf("png")!=-1)//只有png时才支持透明，jpg和纯色不需要
    {
        pix.fill(Qt::transparent);
    }

    QPainter painter(&pix);
    QAbstractTextDocumentLayout::PaintContext ctx;
    ctx.clip = QRectF(pix.rect());
    ui->note->document()->documentLayout()->draw(&painter,ctx);
    //ui->note->document()->drawContents(&painter);//不能画背景

   // pix.save(cfgpath+"/66.png");
    qDebug()<<this->geometry()<<ui->note->size();

    if(item.pre.isEmpty()) item.pre = "1";
    if(item.note.isEmpty()) item.note = "空白事件";
    if(item.color.isEmpty()) item.color = "#ffffff";
    emit setfinished(item,pix);

}


void editnote::on_size_valueChanged(int arg1)
{
    qDebug()<<"fontsizevaluechange"<<arg1;
    size = arg1;
    change(0);
}
void editnote::on_bold_clicked()
{
    change(1);
}
void editnote::on_color_clicked()
{
    color = QColorDialog::getColor(Qt::white, this);
    if(!color.isValid()) return;
  //  qDebug()<<color;
    change(2);

}


void editnote::change(int mode)
{
    textcur = ui->note->textCursor();
    //获取当前选择状态
    //qDebug()<< textcur.position();
    QTextCursor tmp = textcur;
    //tmp备份当前选择状态
    int end = textcur.selectionEnd();
    int start = textcur.selectionStart();
    textcur.movePosition(QTextCursor::Right);//回到最左
    bold =!(textcur.charFormat().font().bold());//取反
    QFont fo;
//    if(textcur.charFormat().font().bold())  fo.setBold(false);
//    else fo.setBold(true);
//    qDebug()<<tmp.selectionStart()<<tmp.selectionEnd();
    for(int i=end;i>start;i--)//最左出发
    {
        textcur.setPosition(i);
        QTextCharFormat fom=textcur.charFormat();

        textcur.movePosition(QTextCursor::Left,QTextCursor::KeepAnchor);
        //qDebug()<< textcur.position();
        switch(mode)
        {
        case 0:
            //设置大小
            fo.setPointSize(size);
            fom.setFont(fo,QTextCharFormat::FontPropertiesSpecifiedOnly);
            break;
        case 1:
            //设置粗体
            fo.setBold(bold);
            fom.setFont(fo,QTextCharFormat::FontPropertiesSpecifiedOnly);
            break;
        case 2:
            fom.setForeground(QBrush(color));
            break;
        default:
            return;
        }
        textcur.setCharFormat( fom);
//        ui->note->setTextCursor(textcur);
        //切换粗体状态

    }
    if(end==start) {
        QTextCharFormat fom=textcur.charFormat();
        fo.setPixelSize(size);
        fo.setBold(bold);
        fom.setFont(fo,QTextCharFormat::FontPropertiesSpecifiedOnly);
        fom.setForeground(QBrush(color));
        textcur.setCharFormat( fom);
        ui->note->setTextCursor(textcur);
        return;
    }
    ui->note->setTextCursor(textcur);

    ui->note->setTextCursor(tmp);

}


void editnote::on_align_activated(int index)
{
    textcur = ui->note->textCursor();
    QTextBlockFormat f= ui->note->textCursor().blockFormat();
    switch (index) {
    case 0:
        f.setAlignment(Qt::AlignHCenter);
        break;
    case 1:
        f.setAlignment(Qt::AlignLeft);
        break;
    case 2:
        f.setAlignment(Qt::AlignRight);
        break;
    default:
        break;
    }
    textcur.setBlockFormat(f);
    ui->note->setTextCursor(textcur);

}

void editnote::on_note_cursorPositionChanged()
{
    textcur = ui->note->textCursor();
    QTextBlockFormat f= ui->note->textCursor().blockFormat();
    QTextCharFormat fom=textcur.charFormat();
    QFont fo = fom.font();
    //ui->size->setValue(fo.pointSize());//这句不能用，用了会触发ui.size的槽函数
   // qDebug()<<"posi"<<fo.pointSize();


    QFont fon;
    fon.setBold(fo.bold());
    ui->bold->setFont(fon);

    QString sty ;
    sty = sty +"QPushButton{color:"+fom.foreground().color().name()+";}";
    ui->color->setStyleSheet(sty);

    switch (f.alignment()) {
    case Qt::AlignHCenter:
        ui->align->setCurrentIndex(0);
        break;
    case Qt::AlignLeft:
        ui->align->setCurrentIndex(1);
        break;
    case Qt::AlignRight:
        ui->align->setCurrentIndex(2);
        break;
    default:
        break;
    }
    //qDebug()<<fom.font().pointSize();

}

void editnote::on_size_editingFinished()//这个用起来不爽
{
//    size = ui->size->value();
//    change(0);
}


void editnote::on_checkBox_clicked(bool checked)
{
    item.isnote = checked;
}

void editnote::on_pushButton_2_clicked()
{
    QColor color;
    color = QColorDialog::getColor(Qt::white, this);
    //qDebug()<<color;
    if(!color.isValid()){
        QFile file(cfgpath+"/bgpic.png");
        if(!file.exists())
        {
            file.setFileName(cfgpath+"/bgpic.jpg");
            if(!file.exists()) color = "#ffffff";//默认白色
            else    item.color=cfgpath+"/bgpic.jpg";

        }
        else item.color=cfgpath+"/bgpic.png";//彩蛋。
    }
    else item.color = color.name();
    setbgcolor(item.color,false);
    qDebug()<<item.color;
}

void editnote::setbgcolor(QString color,bool init)
{

    QTextFrameFormat f;
    f.setMargin(2);
    if(color.indexOf(QRegularExpression("png|jpg"))==-1)
    {
        f.setBackground(QBrush(QColor(color)));
    }
    else {
        QPixmap pix(color);
       if(init) pix = pix.scaled(initsize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
       else pix = pix.scaled(ui->note->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
       f.setBackground(QBrush(QColor(Qt::transparent),pix));
    }
    ui->note->document()->rootFrame()->setFrameFormat(f);

}

void editnote::on_enable_clicked(bool checked)
{
    item.isenable = checked;
}

void editnote::on_cancel_clicked()
{
    emit editcancel();
}
