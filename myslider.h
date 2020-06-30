#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QObject>
#include <QSlider>

class mySlider : public QSlider
{
    Q_OBJECT
public:
    mySlider(QWidget *parent = nullptr);
    ~mySlider();

protected:
    void mousePressEvent(QMouseEvent *ev);

signals:
    void clickChangePosition(int value);

};

#endif // MYSLIDER_H
