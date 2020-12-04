#ifndef SHSECURITYDLG_H
#define SHSECURITYDLG_H

#include <QWidget>
#include <SHControls.h>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "SHSerialPort.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <SHNetworkMessage.h>
#include <SHTcpSocket.h>
extern SHSerialPort *pSHSerial;
namespace Ui {
class SHSecurityDlg;
}

class SHSecurityDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SHSecurityDlg(QWidget *parent = nullptr);
    ~SHSecurityDlg();
    void setNomalStyle();
    void init();
    void updateListWidgetRoom();
    void updateScrollAreaDeviceCtl();

private slots:
    void on_m_btnBack_clicked();
    void on_m_btnRoomEnv_clicked();
    void on_m_listwgRoom_itemClicked(QListWidgetItem *item);


    void on_m_btnWarn_clicked();
    //void readData();

signals:
    void backToLaunchDlg(int dialog);
private:
    Ui::SHSecurityDlg *ui;
    QString m_szListWidgetRoomText;  //记录当前选中的list列表项
    int m_markFunction;
    SHTcpSocket *tcpClient;
};

#endif // SHSECURITYDLG_H
