#include "SHPropertyDlg.h"
#include "ui_SHPropertyDlg.h"
#include <QMessageBox>
#include <SHNetworkMessage.h>
SHPropertyDlg::SHPropertyDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SHPropertyDlg)
{
    ui->setupUi(this);
    setNomalStyle();
    tcpClient = new SHTcpSocket(this);
}

SHPropertyDlg::~SHPropertyDlg()
{
    qDebug()<<"~SHPropertyDlg()";
    delete ui;
}

void SHPropertyDlg::setNomalStyle()
{
    QFile styleSheet(":/qss/style_SHHouseCtlDlg.qss");
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
    if(ui->m_btnConnect->text() == tr("连接"))
    {
        tcpClient->connectToHost(ui->m_EditIP->text(),ui->m_EditPort->text().toShort());
        if(tcpClient->waitForConnected(1000))
        {
            ui->m_btnConnect->setText("断开");

            SHNetworkMessage *info = new SHNetworkMessage;
            info->setMessageType(MessageHeader_MessageType_LOGIN_REQ);
            info->mergeMessage();

            QByteArray data;
            info->serializeToString(data);
            tcpClient->write(data);
        }
    }
    else
    {
        tcpClient->disconnectFromHost();
        if(tcpClient->waitForDisconnected(1000) || tcpClient->state() == QAbstractSocket::UnconnectedState)
        {
            ui->m_btnConnect->setText("连接");
        }
    }
}
