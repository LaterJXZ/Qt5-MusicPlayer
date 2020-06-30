#include "myslider.h"
#include <QMouseEvent>

mySlider::mySlider(QWidget *parent):
    QSlider(parent)
{

}

mySlider::~mySlider()
{

}

void mySlider::mousePressEvent(QMouseEvent *ev)
{
    int curPos = ev->pos().x();
    double percent = curPos * 1.0 / this->width();
    int value = percent * (this->maximum() - this->minimum()) + this->minimum();
    this->setValue(value);
    QSlider::mousePressEvent(ev);
    emit clickChangePosition(value);
}
