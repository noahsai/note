#ifndef EDIT_H
#define EDIT_H

#include <QWidget>
#include<layeritem.h>
#include<QDebug>
#include<QMessageBox>
#include<QTextCursor>
#include<QTextDocumentFragment>
#include<QTextCharFormat>
#include<QColorDialog>
#include<QTextBlock>
#include<QPainter>
#include <QAbstractTextDocumentLayout>

namespace Ui {
class edit;
}

extern QString cfgpath;//全局变量


class editnote : public QWidget
{
    Q_OBJECT

public:
    explicit editnote(QWidget *parent = 0);
    ~editnote();

    void setdata(LayerItem&);
    QSize initsize;

private slots:
    void on_note_textChanged();

    void on_onetime_clicked(bool checked);

    void on_type_currentIndexChanged(int index);

    void on_date_textChanged(const QString &arg1);

    void on_time_textChanged(const QString &arg1);

    void on_pre_textChanged(const QString &arg1);

    void on_onetime_clicked();

    void sethide_show(bool,bool,bool);

    void on_date_editingFinished();

    void on_time_editingFinished();

    void on_pre_editingFinished();

    void on_pushButton_clicked();

    void on_bold_clicked();

    void on_size_valueChanged(int arg1);

    void on_color_clicked();


    void on_align_activated(int index);

    void on_note_cursorPositionChanged();

    void on_size_editingFinished();


    void on_checkBox_clicked(bool checked);

    void on_pushButton_2_clicked();

    void on_enable_clicked(bool checked);

    void on_cancel_clicked();

signals:
    void setfinished(const LayerItem,const QPixmap&);
    void editcancel();

private:
    Ui::edit *ui;
    QTextCursor textcur;
    LayerItem item;
    LayerItem itembk;

    int size;
    bool bold;
    QColor color;//文字颜色
    void change(int );
    void getall();
    void setall();
    void setbgcolor(QString,bool init);
    void inititem();

};

#endif // EDIT_H
