#include "SecurityHumiture.h"
#include "ui_SecurityHumiture.h"
#include <QDebug>
SecurityHumiture::SecurityHumiture(QWidget *parent,QString type) :
    QWidget(parent),
    ui(new Ui::SecurityHumiture)
{
    ui->setupUi(this);
    init(type);

    tcpClient = new SHTcpSocket(this);
    connect(SHSerialPort::getInstance(),SIGNAL(messageHumiture(QString,QString)),this,SLOT(updateHumitureData(QString,QString)));
    connect(SHSerialPort::getInstance(),SIGNAL(messageFireAlarm()),this,SLOT(updateFireAlarm()));
}

SecurityHumiture::~SecurityHumiture()
{
    delete ui;
}

void SecurityHumiture::init(QString type)
{
    ui->m_lb1->hide();
    ui->m_lb1Text->hide();
    ui->m_lb2->hide();
    ui->m_lb2Text->hide();
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("background-color:rgb(255,255,255,20)");
    if(type == "温湿度计")
    {
        ui->m_lb1->show();
        ui->m_lb1Text->show();
        ui->m_lb2->show();
        ui->m_lb2Text->show();
        ui->m_lb1->setText("温度:");
        ui->m_lb2->setText("湿度:");
        ui->m_btnIcon->setStyleSheet("border-image: url(:/images/SecurityDlg/Humiture.png)");
    }
    else if(type == "火焰传感器")
    {
        ui->m_lb1->show();
        ui->m_lb1->setText("正常");
        ui->m_btnIcon->setStyleSheet("border-image: url(:/images/SecurityDlg/FireAlarm.png)");
    }
}

void SecurityHumiture::setDeviceName(QString name)
{
    ui->m_lbName->setText(name);
}

void SecurityHumiture::updateHumitureData(QString temp,QString humi)
{
    ui->m_lb1Text->setText(temp);
    ui->m_lb2Text->setText(humi);
}

void SecurityHumiture::updateFireAlarm()
{
    SHSerialPort::fireAlarmDlgExist = true;
    QMessageBox::StandardButton rb = QMessageBox::warning(this, "", "火灾报警\n是否发送到社区服务中心？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        SHSerialPort::fireAlarmDlgExist = false;
        if(tcpClient->state() != QAbstractSocket::SocketState::ConnectedState)
            tcpClient->connectToHost("192.168.1.11",5000);

        if(tcpClient->waitForConnected(1000))
        {
            SHNetworkMessage *info = new SHNetworkMessage;
            info->setMessageType(MessageHeader_MessageType_ALARMINFO_REQ);
            info->setMessageAlarmType(MessageBody_MessageAlarmType_ALARM_FIRE);
            info->mergeMessage();

            QByteArray data;
            info->serializeToString(data);
            tcpClient->write(data);
        }
        else
        {
        }
    }
    else
    {
        SHSerialPort::fireAlarmDlgExist = false;
    }
}
