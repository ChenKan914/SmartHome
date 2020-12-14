#ifndef SHNEIGHBORDLG_H
#define SHNEIGHBORDLG_H

#include <QWidget>
#include "SHControls.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <SHNetworkMessage.h>
namespace Ui {
class SHNeighborDlg;
}

class SHNeighborDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SHNeighborDlg(QWidget *parent = nullptr);
    ~SHNeighborDlg();
    void setNomalStyle();

private slots:
    void on_m_btnBack_clicked();
    void on_m_cbxBuildPart_currentTextChanged(const QString &arg1);
    void on_m_cbxBuildID_currentIndexChanged(const QString &arg1);
    void on_m_cbxFloor_currentIndexChanged(const QString &arg1);
    void on_m_btResearch_clicked();

    void readData();
    void on_m_btDial_clicked();

private:
    Ui::SHNeighborDlg *ui;
    QTcpSocket *tcpClient;

signals:
    void backToLaunchDlg(int dialog);
};

#endif // SHNEIGHBORDLG_H
