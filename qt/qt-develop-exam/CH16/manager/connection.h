#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QDebug>
#include "qdom.h"

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setHostName("easybook-3313b0");
    db.setDatabaseName("data.db");
    db.setUserName("zhouhejun");
    db.setPassword("123456");
    if(!db.open()){
        //提示出错
        return false;
    }

    QSqlQuery query;

    //创建厂家表
    query.exec("create table factory(id varchar primary key,name varchar)");
    query.exec(QString("insert into factory values('0','请选择厂家')"));
    query.exec(QString("insert into factory values('01','一汽大众')"));
    query.exec(QString("insert into factory values('02','二汽神龙')"));
    query.exec(QString("insert into factory values('03','上海大众')"));

    //创建品牌表
    query.exec("create table brand(id varchar primary key,name varchar,factory varchar,price int,sum int,sell int,last int)");
    query.exec(QString("insert into brand values('01','奥迪A6','一汽大众',36,50,10,40)"));
    query.exec(QString("insert into brand values('02','捷达','一汽大众',34,20,5,15)"));
    query.exec(QString("insert into brand values('03','宝来','一汽大众',41,80,20,60)"));
    query.exec(QString("insert into brand values('04','奔驰','一汽大众',83,40,15,25)"));
    query.exec(QString("insert into brand values('05','毕加索','二汽神龙',39,50,15,35)"));
    query.exec(QString("insert into brand values('06','富康','二汽神龙',28,60,10,50)"));
    query.exec(QString("insert into brand values('07','标致307','二汽神龙',27,70,20,50)"));
    query.exec(QString("insert into brand values('08','桑塔纳','上海大众',25,75,25,50)"));
    query.exec(QString("insert into brand values('09','帕萨特','上海大众',27,65,20,45)"));

    //创建密码表
    query.exec("create table password(pwd varchar primary key)");
    query.exec("insert into password values('123456')");

    return true;
}

static bool createXml()
{
    QFile file("data.xml");
    if(file.exists())return true;
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
        return false;
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement(QString("日销售清单"));
    doc.appendChild(root);
    QTextStream out(&file);
    doc.save(out,4);
    file.close();
    return true;
}

#endif // CONNECTION_H
