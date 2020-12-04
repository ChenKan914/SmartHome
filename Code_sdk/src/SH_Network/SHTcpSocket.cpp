#include "SHTcpSocket.h"
#include <QObject>
SHTcpSocket::SHTcpSocket(QWidget *parent):
    QTcpSocket(parent)
{
    parentDlg = parent;
    connect(this,SIGNAL(readyRead()),this,SLOT(readNetworkData()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(readError(QAbstractSocket::SocketError)));
}
SHTcpSocket::~SHTcpSocket()
{

}
void SHTcpSocket::readNetworkData()
{
    qDebug()<<"SHTcpSocket::readData()";
    QByteArray buf = this->readAll();
    SHNetworkMessage *info = new SHNetworkMessage;
    info->parseMessage(buf);
    int type = info->getMessageType();
    switch (type)
    {
    case MessageHeader_MessageType_ALARMINFO_RESP:
         QMessageBox::warning(parentDlg, "报警", "系统中心已处理", QMessageBox::Yes);
         break;
    case MessageHeader_MessageType_LOGIN_RESP:
         qDebug()<<"login success";
         break;
    default:break;
    }
}

void SHTcpSocket::readError(QAbstractSocket::SocketError)
{
    this->disconnectFromHost();
    QMessageBox msgBox;
    msgBox.setText(tr("failed to connect server because %1").arg(this->errorString()));
    msgBox.exec();
}
