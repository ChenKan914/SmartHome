#include <QApplication>
#include <SHDialogMgrDlg.h>
#include <QDebug>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <SHSerialPort.h>
#include <QObject>
#include "SHNetworkMessage.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
void initSql()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName("localhost");
    db.setDatabaseName("smarthome");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("root");
    if(!db.open())
    {
        qDebug()<<"open failed";
        return;
    }
    else
    {
        qDebug()<<"open succeeded";
    }

    QSqlQuery query(db);
    query.exec("create table devicemgr("
                            "MacAddr char(16) primary key,"
                            "ShortAddr char(4),"
                            "Name char(16),"
                            "Type char(16),"
                            "Location char(16))");


    query.exec("drop table if exists aircondition");

    query.exec("create table aircondition("
                            "MacAddr char(16) primary key,"
                            "Auto1 char(40),"
                            "Auto2 char(32),"
                            "Cold1 char(40),"
                            "Cold2 char(32),"
                            "Wet1 char(40),"
                            "Wet2 char(32),"
                            "Wind1 char(40),"
                            "Wind2 char(32),"
                            "Hot1 char(40),"
                            "Hot2 char(32),"
                            "CurrentCode1 char(40),"
                            "CurrentCode2 char(32))");
    QString audoMode1 = "0001000010010000000001000000101001000000";
    QString coldMode1 = "1001000010010000000001000000101001000000";
    QString WetMode1 = "0101000010010000000001000000101001000000";
    QString windMode1 = "1101000010010000000001000000101001000000";
    QString hotMode1 = "0011000000110000000001000000101001000000";

    QString audoMode2 = "00000000000001000000000000001011";
    QString coldMode2 = "00000000000001000000000000000111";
    QString WetMode2 = "00000000000001000000000000001111";
    QString windMode2 = "00000000000001000000000000000000";
    QString hotMode2 = "00000000000001000000000000000010";

    QString operateCode1 = "0000000010010000000001000000101001000000";
    QString operateCode2 = "00000000000001000000000000001010";

    QString AirConditon = "空调";
    query.exec("select * from devicemgr where Type = '"+AirConditon+"'");
    while(query.next())
    {
        QString Addr = query.value(0).toString();
        query.exec("insert into aircondition("
                   "MacAddr,Auto1,Auto2,Cold1,Cold2,Wet1,Wet2,Wind1,Wind2,Hot1,Hot2,CurrentCode1,CurrentCode2) "
                   "values('"+Addr+"','"+audoMode1+"','"+audoMode2+"','"+coldMode1+"','"+coldMode2+"',"
                   "'"+WetMode1+"','"+WetMode2+"','"+windMode1+"','"+windMode2+"',"
                   "'"+hotMode1+"','"+hotMode2+"','"+operateCode1+"','"+operateCode2+"')");
    }
}

void initSerialPort()
{
    SHSerialPort *pQSerial = SHSerialPort::getInstance();
    pQSerial->setPortName("ttyUSB0");

    if(pQSerial->open(QIODevice::ReadWrite))
    {
        qDebug()<<"ok";
        pQSerial->setBaudRate(QSerialPort::Baud115200);
        pQSerial->setDataBits(QSerialPort::Data8);
        pQSerial->setStopBits(QSerialPort::OneStop);
        pQSerial->setParity(QSerialPort::NoParity);
        pQSerial->setFlowControl(QSerialPort::NoFlowControl);
        pQSerial->waitForReadyRead(100);
    }
    else
    {
        qDebug()<<"error";
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initSql();
    initSerialPort();

    SHDialogMgrDlg w;
    w.show();

    return a.exec();
}
