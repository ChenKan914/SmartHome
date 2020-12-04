#ifndef SHSERIALPORT_H
#define SHSERIALPORT_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>
#include <QBuffer>
#include <SHControls.h>
#include "HouseCtlConditon.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QTimer>
extern QSerialPort *pQSerial;
namespace Ui {
class SHSerialPort;
}

class SHSerialPort : public QWidget
{
    Q_OBJECT

public:
    explicit SHSerialPort(QWidget *parent = nullptr);
    ~SHSerialPort();


    void serialDataHandle(QByteArray  serial_buf);
    bool serialDataCorrect(QByteArray data);
    void msgProcessEvent(changeMsg msg);

    void eventAddrHandle(changeMsg msg);
    void eventDHT11Msg(changeMsg msg);


private slots:
    void readData();

    void on_m_btnOpen_clicked();
    void on_m_btnClr_clicked();
    void on_m_btnSendData_clicked();
    void on_m_btnRefresh_clicked();

    void updateReceiveMsg();

signals:
    void sendSerialMessage();
    void messageHumiture(QString temp,QString humi);
private:
    Ui::SHSerialPort *ui;
    bool m_receiveProMsg;
    QTimer *m_timer;
};
#endif // SHSERIALPORT_H
