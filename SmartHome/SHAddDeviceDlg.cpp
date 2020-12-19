#include "SHAddDeviceDlg.h"
#include "ui_SHAddDeviceDlg.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>


SHAddDeviceDlg::SHAddDeviceDlg(QWidget *parent,QString ShortAddr,QString DeviceAddr,QString DeviceName,QString DeviceType,QString DeviceLocate) :
    QDialog(parent),
    ui(new Ui::SHAddDeviceDlg)
{
    ui->setupUi(this);

    init(DeviceAddr,DeviceName,DeviceType,DeviceLocate);
    m_shortAddr = ShortAddr;
}

SHAddDeviceDlg::~SHAddDeviceDlg()
{
    qDebug()<<"~SHAddDeviceDlg";
    delete ui;
}

void SHAddDeviceDlg::init(QString Addr,QString Name,QString Type,QString Locate)
{
    QStringList DeviceSortList,DeviceRoomList;
    DeviceSortList<<"电灯"<<"空调"<<"窗帘"<<"电视"<<"温湿度计"<<"火焰传感器"<<"烟雾传感器";
    DeviceRoomList<<"客厅"<<"卧室1"<<"卧室2"<<"阳台"<<"卫生间";
    ui->m_DeviceSort->addItems(DeviceSortList);
    ui->m_DeviceLocate->addItems(DeviceRoomList);

    ui->m_DeviceAddr->setEnabled(false);
    ui->m_DeviceAddr->setText(Addr);
    ui->m_DeviceName->setText(Name);
    ui->m_DeviceSort->setCurrentText(Type);
    ui->m_DeviceLocate->setCurrentText(Locate);
}

void SHAddDeviceDlg::updateLabelTitle(QString label)
{
    ui->m_lbTitle->setText(label);
}
void SHAddDeviceDlg::on_buttonBox_accepted()
{
    QString Addr = ui->m_DeviceAddr->text();
    QString Name = ui->m_DeviceName->text();
    QString Type = ui->m_DeviceSort->currentText();
    QString Locate = ui->m_DeviceLocate->currentText();

    QSqlQuery query;
    query.exec("select * from devicemgr where MacAddr = '"+Addr+"'");
    if(query.next() == NULL)
    {
        query.exec("insert into devicemgr(MacAddr,ShortAddr,Name,Type,Location) values('"+Addr+"','"+m_shortAddr+"','"+Name+"','"+Type+"','"+Locate+"')");
    }
    else
    {
        query.exec("update devicemgr set Name = '"+Name+"' where MacAddr = '"+Addr+"'");
        query.exec("update devicemgr set Type = '"+Type+"' where MacAddr = '"+Addr+"'");
        query.exec("update devicemgr set Location = '"+Locate+"' where MacAddr = '"+Addr+"'");
    }
    emit btnClickSignal(true);
    this->hide();
}

void SHAddDeviceDlg::on_buttonBox_rejected()
{
    emit btnClickSignal(false);
    this->hide();
}
