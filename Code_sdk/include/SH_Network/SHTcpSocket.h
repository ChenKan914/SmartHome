#ifndef SHTCPSOCKET_H
#define SHTCPSOCKET_H
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <SHNetworkMessage.h>
#include <QMessageBox>
class SHTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    SHTcpSocket();
    SHTcpSocket(QWidget *parent = Q_NULLPTR);
    virtual ~SHTcpSocket();

private:
    QWidget *parentDlg;
private slots:
    void readNetworkData();
    void readError(QAbstractSocket::SocketError);

private:
};

#endif // SHTCPSOCKET_H
