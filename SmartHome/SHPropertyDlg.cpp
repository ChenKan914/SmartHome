#include "SHPropertyDlg.h"
#include "ui_SHPropertyDlg.h"
#include <QMessageBox>
#include <SHNetworkMessage.h>
SHPropertyDlg::SHPropertyDlg(QWidget *parent) :
    QWidget(parent),m_bconnected(false),
    ui(new Ui::SHPropertyDlg)
{
    ui->setupUi(this);
    setNomalStyle();
    tcpClient = new SHTcpSocket(this);

    ui->m_btnNotice->setDisabled(true);
    ui->m_btnMessage->setDisabled(true);
    ui->m_btnDial->setDisabled(true);
}

SHPropertyDlg::~SHPropertyDlg()
{
    qDebug()<<"~SHPropertyDlg()";
    delete ui;
}

void SHPropertyDlg::setNomalStyle()
{
    QFile styleSheet(":/qss/style_SHPropertyDlg.qss");
    if (!styleSheet.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");
    }
    else
    {
        this->setStyleSheet(styleSheet.readAll());
    }
}

void SHPropertyDlg::on_m_btnBack_clicked()
{
    emit backToLaunchDlg(SHEnumMgr::SH_LAUNCHDLG);
}

void SHPropertyDlg::on_m_btnConnect_clicked()
{
    if(m_bconnected == false)
    {
        tcpClient->connectToHost("192.168.179.128",5000);
        if(tcpClient->waitForConnected(1000))
        {
            SHNetworkMessage *info = new SHNetworkMessage;
            info->setMessageType(MessageHeader_MessageType_LOGIN_REQ);
            info->mergeMessage();

            QByteArray data;
            info->serializeToString(data);
            tcpClient->write(data);
            m_bconnected = true;
            ui->m_btnNotice->setDisabled(false);
            ui->m_btnMessage->setDisabled(false);
            ui->m_btnDial->setDisabled(false);
        }
    }
    else
    {
        tcpClient->disconnectFromHost();
        if(tcpClient->waitForDisconnected(1000) || tcpClient->state() == QAbstractSocket::UnconnectedState)
        {
            m_bconnected = false;
            ui->m_btnNotice->setDisabled(true);
            ui->m_btnMessage->setDisabled(true);
            ui->m_btnDial->setDisabled(true);
        }
    }


}
