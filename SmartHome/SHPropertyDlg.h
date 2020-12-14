#ifndef SHPROPERTYDLG_H
#define SHPROPERTYDLG_H

#include <QWidget>
#include "SHControls.h"
#include <QFile>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <SHTcpSocket.h>
namespace Ui {
class SHPropertyDlg;
}

class SHPropertyDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SHPropertyDlg(QWidget *parent = nullptr);
    ~SHPropertyDlg();
    void setNomalStyle();

private slots:
    void on_m_btnBack_clicked();
    void on_m_btnConnect_clicked();

signals:
    void backToLaunchDlg(int dialog);

private:
    Ui::SHPropertyDlg *ui;
    SHTcpSocket *tcpClient;
};

#endif // SHPROPERTYDLG_H
