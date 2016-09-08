#ifndef EDIT_H
#define EDIT_H

#include <QWidget>
#include<layeritem.h>
#include<QDebug>
namespace Ui {
class edit;
}

class edit : public QWidget
{
    Q_OBJECT

public:
    explicit edit(QWidget *parent = 0);
    ~edit();

    void setdata(LayerItem&);
    LayerItem item;

private slots:
    void on_note_textChanged(const QString &arg1);

    void on_onetime_clicked(bool checked);

    void on_type_currentIndexChanged(int index);

    void on_date_textChanged(const QString &arg1);

    void on_time_textChanged(const QString &arg1);

    void on_pre_textChanged(const QString &arg1);

    void on_onetime_clicked();

    void sethide_show(bool,bool,bool);

private:
    Ui::edit *ui;
};

#endif // EDIT_H
