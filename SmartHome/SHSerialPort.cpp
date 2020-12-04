#include "SHSerialPort.h"
#include "ui_SHSerialPort.h"
#include <SHAddDeviceDlg.h>
#include <QMessageBox>
#include <SHNetworkMessage.h>
SHSerialPort::SHSerialPort(QWidget *parent) :
    m_receiveProMsg(0),QWidget(parent),
    ui(new Ui::SHSerialPort)
{
    ui->setupUi(this);
    //查找串口
    foreach ( const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->m_cboxPort->addItem(serial.portName());
            serial.close();
        }
    }
//    if(ui->m_cboxPort->currentText() == nullptr)
//    {
//        ui->m_btnOpen->setEnabled(false);
//        ui->m_btnClr->setEnabled(false);
//        ui->m_btnSendData->setEnabled(false);
//    }
//    else
//    {
//        ui->m_btnOpen->setEnabled(true);
//        ui->m_btnClr->setEnabled(true);
//        ui->m_btnSendData->setEnabled(true);
//    }

//    ui->m_cboxBaud->setCurrentIndex(7);
//    ui->m_cboxBitNum->setCurrentIndex(3);

    connect(pQSerial,SIGNAL(readyRead()),this,SLOT(readData()));

    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(updateReceiveMsg()));
}

SHSerialPort::~SHSerialPort()
{
    delete ui;
}

void SHSerialPort::on_m_btnOpen_clicked()
{
    if(ui->m_btnOpen->text() == tr("打开串口"))
    {
        pQSerial = new QSerialPort;
        pQSerial->setPortName(ui->m_cboxPort->currentText());
        pQSerial->open(QIODevice::ReadWrite);

        pQSerial->setBaudRate(ui->m_cboxBaud->currentText().toInt());
        pQSerial->setDataBits(QSerialPort::Data8);
        pQSerial->setStopBits(QSerialPort::OneStop);
        pQSerial->setParity(QSerialPort::NoParity);
        pQSerial->setFlowControl(QSerialPort::NoFlowControl);

        ui->m_cboxPort->setEnabled(false);
        ui->m_cboxBaud->setEnabled(false);
        ui->m_cboxBitNum->setEnabled(false);
        ui->m_cboxStop->setEnabled(false);
        ui->m_cboxStop->setEnabled(false);
        ui->m_cboxParity->setEnabled(false);

        ui->m_btnOpen->setText("关闭串口");
        ui->m_btnRefresh->setEnabled(false);

        connect(pQSerial,SIGNAL(readyRead()),this,SLOT(readData()));
    }
    else
    {
        pQSerial->close();

        ui->m_cboxPort->setEnabled(true);
        ui->m_cboxBaud->setEnabled(true);
        ui->m_cboxBitNum->setEnabled(true);
        ui->m_cboxStop->setEnabled(true);
        ui->m_cboxStop->setEnabled(true);
        ui->m_cboxParity->setEnabled(true);

        ui->m_btnOpen->setText("打开串口");
        ui->m_btnRefresh->setEnabled(true);
    }

}

void SHSerialPort::on_m_btnClr_clicked()
{
    ui->m_textRev->clear();
}

void SHSerialPort::on_m_btnSendData_clicked()
{
    pQSerial->write(ui->m_textSend->toPlainText().toLatin1());
}

void SHSerialPort::on_m_btnRefresh_clicked()
{
    ui->m_cboxPort->clear();
    //查找串口
    foreach (const QSerialPortInfo  &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->m_cboxPort->addItem(serial.portName());
            serial.close();
        }
    }

    if(ui->m_cboxPort->currentText() == nullptr)
    {
        ui->m_btnOpen->setEnabled(false);
        ui->m_btnClr->setEnabled(false);
        ui->m_btnSendData->setEnabled(false);
    }
    else
    {
        ui->m_btnOpen->setEnabled(true);
        ui->m_btnClr->setEnabled(true);
        ui->m_btnSendData->setEnabled(true);
    }
}

void SHSerialPort::readData()
{
    QByteArray buf;
    buf = pQSerial->readAll();
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
    SHAddDeviceDlg *addDevice = new SHAddDeviceDlg(this,msg.shortAddr,msg.extAddr);
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

void SHSerialPort::updateReceiveMsg()
{
    qDebug()<<"updateReceiveMsg";
    m_receiveProMsg = 0;
    m_timer->stop();
}
