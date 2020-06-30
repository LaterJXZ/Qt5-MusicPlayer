#ifndef CONNECTDB_H
#define CONNECTDB_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

static bool CreateConnection(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("myMusic.db");
    if(!db.open())
        return false;
    QSqlQuery query;
    //本地列表
    query.exec(QString("create table localMusic(id int,MusicName varchar,FileName varchar,tag int,primary key(id,MusicName,FileName))"));
    //喜爱列表
    query.exec(QString("create table likeMusic(id int,MusicName varchar,FileName varchar,primary key(id,MusicName,FileName))"));
    //播放队列数据
    query.exec(QString("create table playMusic(id int,MusicName varchar,FileName varchar,primary key(id,MusicName,FileName))"));
    //壁纸数据
    query.exec(QString("create table wallPaper(FileName varchar primary key)"));
    return true;
}

#endif // CONNECTDB_H
