#include "mylrc.h"
#include "ui_mylrc.h"

myLrc::myLrc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myLrc)
{
    ui->setupUi(this);
    ui->label->setWindowFlag(Qt::WindowStaysOnTopHint);
}

void myLrc::setText(QString text)
{
    ui->label->setText(text);
}

QString myLrc::text()
{
    return ui->label->text();
}

myLrc::~myLrc()
{
    delete ui;
}
