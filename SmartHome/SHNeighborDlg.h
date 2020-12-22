#ifndef SHNEIGHBORDLG_H
#define SHNEIGHBORDLG_H

#include <QWidget>
#include "SHControls.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <SHNetworkMessage.h>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QTableWidgetItem>
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
    void updateTableWidget();

private slots:
    void on_m_btnBack_clicked();
    void on_m_cbxBuildPart_currentTextChanged(const QString &arg1);
    void on_m_cbxBuildID_currentIndexChanged(const QString &arg1);
    void on_m_cbxFloor_currentIndexChanged(const QString &arg1);
    void on_m_btResearch_clicked();

    void readData();
    void on_m_btDial_clicked();

    void on_m_tbwgHistory_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

private:
    Ui::SHNeighborDlg *ui;
    QTcpSocket *tcpClient;

    int insertTableFromSelect;

signals:
    void backToLaunchDlg(int dialog);
};

#endif // SHNEIGHBORDLG_H
