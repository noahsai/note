#ifndef NOTE_H
#define NOTE_H

#include <QWidget>

namespace Ui {
class Note;
}

class Note : public QWidget
{
    Q_OBJECT

public:
    explicit Note(QWidget *parent = 0);
    ~Note();

private:
    Ui::Note *ui;
};

#endif // NOTE_H
