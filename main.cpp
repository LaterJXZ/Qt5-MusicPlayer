#include "widget.h"
#include "connectDB.h"
#include "myslider.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!CreateConnection()){
        return 1;
    }
    Widget w;
    w.setWindowIcon(QIcon("title.icon.ico"));
    //w.setWindowFlags(Qt::FramelessWindowHint);
    w.setWindowTitle("Later音乐");
    w.show();
    return a.exec();
}
