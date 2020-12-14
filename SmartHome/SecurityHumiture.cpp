#include "SecurityHumiture.h"
#include "ui_SecurityHumiture.h"
#include <QDebug>
SecurityHumiture::SecurityHumiture(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecurityHumiture)
{
    ui->setupUi(this);
    init();

    connect(SHSerialPort::getInstance(),SIGNAL(messageHumiture(QString,QString)),this,SLOT(updateHumitureData(QString,QString)));
}

SecurityHumiture::~SecurityHumiture()
{
    delete ui;
}

void SecurityHumiture::init()
{
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("background-color:rgb(255,255,255,20)");
    ui->m_btnIcon->setStyleSheet("border-image: url(:/images/SecurityDlg/Humiture.png)");
}

void SecurityHumiture::setDeviceName(QString name)
{
    ui->m_lbName->setText(name);
}

void SecurityHumiture::updateHumitureData(QString temp,QString humi)
{
    ui->m_lbTemp->setText(temp);
    ui->m_lbHumi->setText(humi);
}
