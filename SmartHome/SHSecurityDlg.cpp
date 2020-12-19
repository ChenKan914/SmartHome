#include "SHSecurityDlg.h"
#include "ui_SHSecurityDlg.h"
#include "SecurityHumiture.h"
#include <QMessageBox>

SHSecurityDlg::SHSecurityDlg(QWidget *parent) :
    QWidget(parent),m_markFunction(0),
    ui(new Ui::SHSecurityDlg)
{
    ui->setupUi(this);
    setNomalStyle();
    init();

    tcpClient = new SHTcpSocket(this);
}

SHSecurityDlg::~SHSecurityDlg()
{
    delete ui;
}

void SHSecurityDlg::setNomalStyle()
{
    QFile styleSheet(":/qss/style_SHSecurityDlg.qss");
    if (!styleSheet.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");
    }
    else
    {
        this->setStyleSheet(styleSheet.readAll());
    }
}

void SHSecurityDlg::init()
{
    ui->m_wgtLaunch->show();
    ui->m_wgtRoomEnv->hide();
}

void SHSecurityDlg::on_m_btnBack_clicked()
{
    if(m_markFunction == 0)
    {
        emit backToLaunchDlg(SHEnumMgr::SH_LAUNCHDLG);
    }
    else
    {
        m_markFunction = 0;
        ui->m_wgtLaunch->show();
        ui->m_wgtRoomEnv->hide();
    }
}

void SHSecurityDlg::on_m_btnRoomEnv_clicked()
{
    m_markFunction = 1;
    ui->m_wgtLaunch->hide();
    ui->m_wgtRoomEnv->show();
    updateListWidgetRoom();

}

void SHSecurityDlg::updateListWidgetRoom()
{
    ui->m_listwgRoom->clear();

    QSqlQuery query;
    query.exec("select Location from devicemgr");
    SHStringList m_listRoomSort;
    while(query.next())
    {
        m_listRoomSort<<query.value(0).toString();
    }
    m_listRoomSort.removeRepeatItem();

    //刷新list列表，默认选中list第一项
    if(m_listRoomSort.count() > 0)
    {
        ui->m_listwgRoom->addItems(m_listRoomSort);
        ui->m_listwgRoom->setItemSelected(ui->m_listwgRoom->item(0),true);
        m_szListWidgetRoomText = m_listRoomSort.at(0);
    }
    updateScrollAreaDeviceCtl();
}

void SHSecurityDlg::updateScrollAreaDeviceCtl()
{
    QList<SecurityHumiture*> lists = ui->m_scrollAreaDevice->findChildren<SecurityHumiture*>();
    foreach (SecurityHumiture* list, lists) {   delete list;  }
    QSqlQuery query;
    query.exec("select * from devicemgr where Location = '"+m_szListWidgetRoomText+"'");

    int index_y = 0;
    while(query.next())
    {
        QString Addr = query.value(0).toString();
        QString ShortAddr = query.value(1).toString();
        QString Name = query.value(2).toString();
        QString Type = query.value(3).toString();

        if(Type == "温湿度计")
        {
            SecurityHumiture *pHumiture = new SecurityHumiture(ui->m_scrollAreaWgDevice,Type);
            pHumiture->setGeometry(5,5 + index_y*100,735,80);
            pHumiture->show();
            pHumiture->setDeviceName(Name);
            index_y++;
            ui->m_scrollAreaWgDevice->setGeometry(0,0,ui->m_scrollAreaDevice->width()-20,5+90*index_y);
        }
        else if(Type == "火焰传感器")
        {
            SecurityHumiture *pHumiture = new SecurityHumiture(ui->m_scrollAreaWgDevice,Type);
            pHumiture->setGeometry(5,5+ index_y*100,735,80);
            pHumiture->show();
            pHumiture->setDeviceName(Name);
            index_y++;
            ui->m_scrollAreaWgDevice->setGeometry(0,0,ui->m_scrollAreaDevice->width()-20,5+90*index_y);
        }
        else if(Type == "烟雾传感器")
        {
            SecurityHumiture *pHumiture = new SecurityHumiture(ui->m_scrollAreaWgDevice,Type);
            pHumiture->setGeometry(5,5+ index_y*100,735,80);
            pHumiture->show();
            pHumiture->setDeviceName(Name);
            index_y++;
            ui->m_scrollAreaWgDevice->setGeometry(0,0,ui->m_scrollAreaDevice->width()-20,5+90*index_y);
        }
    }
}

void SHSecurityDlg::on_m_listwgRoom_itemClicked(QListWidgetItem *item)
{
    m_szListWidgetRoomText = item->text();
    updateListWidgetRoom();
}

void SHSecurityDlg::on_m_btnWarn_clicked()
{
    QMessageBox::StandardButton rb = QMessageBox::warning(this, "", "紧急报警", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        if(tcpClient->state() != QAbstractSocket::SocketState::ConnectedState)
            tcpClient->connectToHost("192.168.1.11",5000);

        if(tcpClient->waitForConnected(1000))
        {
            SHNetworkMessage *info = new SHNetworkMessage;
            info->setMessageType(MessageHeader_MessageType_ALARMINFO_REQ);
            info->setMessageAlarmType(MessageBody_MessageAlarmType_ALARM_KEY);
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
    }
}
