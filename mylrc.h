#ifndef MYLRC_H
#define MYLRC_H

#include <QWidget>

namespace Ui {
class myLrc;
}

class myLrc : public QWidget
{
    Q_OBJECT

public:
    explicit myLrc(QWidget *parent = nullptr);
    void setText(QString text);
    QString text();
    ~myLrc();

private:
    Ui::myLrc *ui;
};

#endif // MYLRC_H
