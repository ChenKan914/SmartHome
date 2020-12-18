#include "SHSerialPort.h"
#include "ui_SHSerialPort.h"
#include <SHAddDeviceDlg.h>
#include <QMessageBox>
#include <SHNetworkMessage.h>

bool SHSerialPort::fireAlarmDlgExist = false;
SHSerialPort* SHSerialPort::instance = nullptr;
SHSerialPort* SHSerialPort::getInstance()
{
    if(instance == nullptr)
    {
        instance = new SHSerialPort();
    }
    return instance;
}

SHSerialPort::SHSerialPort()
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readData()));
}

SHSerialPort::~SHSerialPort()
{
}
void SHSerialPort::readData()
{
    QByteArray buf;
    buf = this->readAll();
    qDebug()<<"echo buf";
    qDebug()<<buf;

    serialDataHandle(buf);
}

void SHSerialPort::serialDataHandle(QByteArray data)
{
    if((data.at(0) == '\xff')&&(data.at(data.size() - 1) == '\x11'))
    {
        changeMsg msg;
        data.remove(0,1);
        data.remove(data.size() - 1,1);
        QString dataString = data.toHex();

        msg.shortAddr = dataString.mid(0,4);
        dataString.remove(0,4);

        msg.extAddr = dataString.mid(0,16);
        dataString.remove(0,16);

        msg.msgType = dataString.mid(0,2).toInt();
        dataString.remove(0,2);

        msg.lenth = dataString.mid(0,2).toInt();
        dataString.remove(0,2);

        msg.data = dataString;

        qDebug()<<"shortAddr:"<<msg.shortAddr;
        qDebug()<<"extAddr:"<<msg.extAddr;
        qDebug()<<"msgType:"<<msg.msgType;
        qDebug()<<"lenth:"<<msg.lenth;
        qDebug()<<"data:"<<msg.data;

        msgProcessEvent(msg);
    }
    else
    {
    }
}

void SHSerialPort::msgProcessEvent(changeMsg msg)
{
    switch(msg.msgType)
    {
    case SMARTHOME_MACADDRMSG:
        eventAddrHandle(msg);
        break;
    case SMARTHOME_TEMPHUMIMSG:
        eventDHT11Msg(msg);
        emit sendSerialMessage();
        break;
    case SMARTHOME_FIREALARM:
        eventFireAlarmMsg(msg);
        break;
    default:
        break;
    }
}

void SHSerialPort::eventAddrHandle(changeMsg msg)
{
    QString shortAddr = msg.shortAddr;
    QString extAddr = msg.extAddr;
    QSqlQuery query;
    query.exec("select MacAddr from devicemgr");
    while (query.next())
    {
        QString addr = query.value(0).toString();
        if(addr == extAddr)
        {
            qDebug()<<"MacAddr has exist~"<<endl;
            query.exec("update devicemgr set ShortAddr = '"+msg.shortAddr+"' where MacAddr = '"+msg.extAddr+"'");
            return ;
        }
    }
    SHAddDeviceDlg *addDevice = new SHAddDeviceDlg(nullptr,msg.shortAddr,msg.extAddr);
    addDevice->show();
}

void SHSerialPort::eventDHT11Msg(changeMsg msg)
{
    QString temp = msg.data.mid(0,2);
    QString hum = msg.data.mid(2,4);
    emit messageHumiture(temp,hum);
    qDebug()<<"temp is :"<<temp;
    qDebug()<<"hun is :"<<hum;
}

void SHSerialPort::eventFireAlarmMsg(changeMsg msg)
{
    if(fireAlarmDlgExist == false)
    {
        emit messageFireAlarm();
    }
}
