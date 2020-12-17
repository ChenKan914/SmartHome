#include "SecurityHumiture.h"
#include "ui_SecurityHumiture.h"
#include <QDebug>
SecurityHumiture::SecurityHumiture(QWidget *parent,QString type) :
    QWidget(parent),
    ui(new Ui::SecurityHumiture)
{
    ui->setupUi(this);
    init(type);

    connect(SHSerialPort::getInstance(),SIGNAL(messageHumiture(QString,QString)),this,SLOT(updateHumitureData(QString,QString)));
    connect(SHSerialPort::getInstance(),SIGNAL(messageFireAlarm()),this,SLOT(updateHumitureData()));
}

SecurityHumiture::~SecurityHumiture()
{
    delete ui;
}

void SecurityHumiture::init(QString type)
{
    ui->m_lb1->hide();
    ui->m_lb1Text->hide();
    ui->m_lb2->hide();
    ui->m_lb2Text->hide();
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("background-color:rgb(255,255,255,20)");
    if(type == "温湿度计")
    {
        ui->m_lb1->show();
        ui->m_lb1Text->show();
        ui->m_lb2->show();
        ui->m_lb2Text->show();
        ui->m_lb1->setText("温度:");
        ui->m_lb2->setText("湿度:");
        ui->m_btnIcon->setStyleSheet("border-image: url(:/images/SecurityDlg/Humiture.png)");
    }
    else if(type == "火焰传感器")
    {
        ui->m_lb1->show();
        ui->m_lb1->setText("正常");
        ui->m_btnIcon->setStyleSheet("border-image: url(:/images/SecurityDlg/FireAlarm.png)");
    }
}

void SecurityHumiture::setDeviceName(QString name)
{
    ui->m_lbName->setText(name);
}

void SecurityHumiture::updateHumitureData(QString temp,QString humi)
{
    ui->m_lb1Text->setText(temp);
    ui->m_lb2Text->setText(humi);
}

void SecurityHumiture::updateFireAlarm()
{
    QMessageBox::StandardButton rb = QMessageBox::warning(this, "", "你确定要删除吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
    }
    else
    {
    }
}
