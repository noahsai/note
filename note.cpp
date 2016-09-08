#include "note.h"
#include "ui_note.h"

Note::Note(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Note)
{
    ui->setupUi(this);
}

Note::~Note()
{
    delete ui;
}
