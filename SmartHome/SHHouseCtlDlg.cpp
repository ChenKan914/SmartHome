#include "SHHouseCtlDlg.h"
#include "ui_SHHouseCtlDlg.h"

SHHouseCtlDlg::SHHouseCtlDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SHHouseCtlDlg)
{
    ui->setupUi(this);
    init();
}

SHHouseCtlDlg::~SHHouseCtlDlg()
{
    delete ui;
}

void SHHouseCtlDlg::init()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setNomalStyle();
    updateListWidgetRoom();

    //等待Widget正常初始化，初始化ScrollArea
    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(updateScrollAreaFirst()));
    m_timer->start(100);
}

void SHHouseCtlDlg::setNomalStyle()
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

void SHHouseCtlDlg::updateScrollAreaFirst()
{
    if(ui->m_scrollAreaDevice->width() <200)
    {
        m_timer->start(100);
    }
    else
    {
        updateScrollAreaDeviceCtl();
        m_timer->stop();
    }
}

void SHHouseCtlDlg::updateListWidgetRoom()
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
    updateListWidgetDevice();
}

void SHHouseCtlDlg::updateListWidgetDevice()
{
    ui->m_listwgDevice->clear();

    QSqlQuery query;
    query.exec("select Type from devicemgr where Location = '"+m_szListWidgetRoomText+"'");
    SHStringList m_listDeviceSort;
    while (query.next())
    {
        if(query.value(0).toString() !="温湿度计" && query.value(0).toString() !="火焰传感器" && query.value(0).toString() !="烟雾传感器")
        {
            m_listDeviceSort<<query.value(0).toString();
        }
    }
    m_listDeviceSort.removeRepeatItem();

    //刷新list列表，默认选中list第一项
    if(m_listDeviceSort.count() > 0)
    {
        ui->m_listwgDevice->addItems(m_listDeviceSort);
        ui->m_listwgDevice->setItemSelected(ui->m_listwgDevice->item(0),true);
        m_szListWidgetDeviceText = m_listDeviceSort.at(0);
    }
    updateScrollAreaDeviceCtl();
}

void SHHouseCtlDlg::updateScrollAreaDeviceCtl()
{
    clearScrollAreaDeviceCtl();

    QSqlQuery query;
    query.exec("select * from devicemgr where Location = '"+m_szListWidgetRoomText+"' and Type = '"+m_szListWidgetDeviceText+"'");

    int index_y = 0;
    while(query.next())
    {
        QString Addr = query.value(0).toString();
        QString ShortAddr = query.value(1).toString();
        QString Name = query.value(2).toString();

        if(m_szListWidgetDeviceText == "电灯")
        {
            HouseCtlLamp *m_pLamp = new HouseCtlLamp(ui->m_scrollAreaWgDevice,Addr,ShortAddr,Name);
            m_pLamp->setGeometry(5,5+82*index_y,540,70);
            m_pLamp->show();
            index_y++;
            ui->m_scrollAreaWgDevice->setGeometry(0,0,ui->m_scrollAreaDevice->width()-20,5+82*index_y);
        }
        else if(m_szListWidgetDeviceText == "窗帘")
        {
            HouseCtlCurtain *m_pCurtain = new HouseCtlCurtain(ui->m_scrollAreaWgDevice,Addr,ShortAddr,Name);
            m_pCurtain->setGeometry(5,5+82*index_y,540,70);
            m_pCurtain->show();
            index_y++;
            ui->m_scrollAreaWgDevice->setGeometry(0,0,ui->m_scrollAreaDevice->width()-20,5+82*index_y);
        }
        else if(m_szListWidgetDeviceText == "空调")
        {
            HouseCtlConditon *m_pCondition = new HouseCtlConditon(ui->m_scrollAreaWgDevice,Addr,ShortAddr,Name);
            m_pCondition->setGeometry(5,5+245*index_y,540,240);
            m_pCondition->show();
            index_y++;
            ui->m_scrollAreaWgDevice->setGeometry(0,0,ui->m_scrollAreaDevice->width()-20,5+245*index_y);
        }
    }
}

void SHHouseCtlDlg::clearScrollAreaDeviceCtl()
{
    QList<HouseCtlLamp*> Lamps = ui->m_scrollAreaWgDevice->findChildren<HouseCtlLamp*>();
    foreach (HouseCtlLamp* Lamp, Lamps) {   delete Lamp;  }
    QList<HouseCtlCurtain*> Curtains = ui->m_scrollAreaWgDevice->findChildren<HouseCtlCurtain*>();
    foreach (HouseCtlCurtain* Curtain, Curtains) {   delete Curtain;  }
    QList<HouseCtlConditon*> Conditions = ui->m_scrollAreaWgDevice->findChildren<HouseCtlConditon*>();
    foreach (HouseCtlConditon* Condition, Conditions) {   delete Condition;  }
}

void SHHouseCtlDlg::on_m_listwgRoom_itemClicked(QListWidgetItem *item)
{
    m_szListWidgetRoomText = item->text();
    updateListWidgetDevice();
}

void SHHouseCtlDlg::on_m_listwgDevice_itemClicked(QListWidgetItem *item)
{
    m_szListWidgetDeviceText = item->text();
    updateScrollAreaDeviceCtl();
}

void SHHouseCtlDlg::on_m_btnBack_clicked()
{
    emit  backToLaunchDlg(SHEnumMgr::SH_LAUNCHDLG);
}
