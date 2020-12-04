#include "SHNeighborDlg.h"
#include "ui_SHNeighborDlg.h"
#include <QFile>
#include<QProcess>

SHNeighborDlg::SHNeighborDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SHNeighborDlg)
{
    setNomalStyle();
    ui->setupUi(this);

    tcpClient = new QTcpSocket;
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(readData()));
    on_m_cbxBuildPart_currentTextChanged("");
}

void SHNeighborDlg::setNomalStyle()
{
    QFile styleSheet(":/qss/style_SHNeighborDlg.qss");
    if (!styleSheet.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");
    }
    else
    {
        this->setStyleSheet(styleSheet.readAll());
    }
}

SHNeighborDlg::~SHNeighborDlg()
{
    delete ui;
}

void SHNeighborDlg::on_m_btnBack_clicked()
{
    emit backToLaunchDlg(SHEnumMgr::SH_LAUNCHDLG);
}


void SHNeighborDlg::on_m_cbxBuildPart_currentTextChanged(const QString &arg1)
{
    if(ui->m_cbxBuildPart->currentText()== "西区")
    {
        ui->m_cbxBuildID->clear();
        QStringList bulidIDList;
        bulidIDList<<"教一楼"<<"教二楼"<<"60栋"<<"新峰公寓";
        ui->m_cbxBuildID->addItems(bulidIDList);
    }
    else if(ui->m_cbxBuildPart->currentText()== "东区")
    {
        ui->m_cbxBuildID->clear();
        QStringList bulidIDList;
        bulidIDList<<"教职工宿舍"<<"8栋"<<"2栋"<<"东教楼"<<"八角楼";
        ui->m_cbxBuildID->addItems(bulidIDList);
    }
    else
    {
        ui->m_cbxBuildID->clear();
        QStringList bulidIDList;
        bulidIDList<<"北综楼";
        ui->m_cbxBuildID->addItems(bulidIDList);
    }
}

void SHNeighborDlg::on_m_cbxBuildID_currentIndexChanged(const QString &arg1)
{
    if(ui->m_cbxBuildID->currentText()== "教一楼" || ui->m_cbxBuildID->currentText()== "教二楼" || ui->m_cbxBuildID->currentText()== "北综楼")
    {
        ui->m_cbxCellID->clear();
    }
    else if(ui->m_cbxBuildID->currentText()== "教职工宿舍")
    {
        ui->m_cbxCellID->clear();
        QStringList bulidIDList;
        bulidIDList<<"1栋"<<"2栋"<<"3栋"<<"4栋";
        ui->m_cbxCellID->addItems(bulidIDList);
    }
    else if(ui->m_cbxBuildID->currentText()== "新峰公寓" || ui->m_cbxBuildID->currentText()== "东教楼")
    {
        ui->m_cbxCellID->clear();
        QStringList bulidIDList;
        bulidIDList<<"A区"<<"B区"<<"C区";
        ui->m_cbxCellID->addItems(bulidIDList);
    }
    else ui->m_cbxCellID->clear();

    ui->m_cbxFloor->clear();
    QStringList bulidIDList;
    bulidIDList<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7";
    ui->m_cbxFloor->addItems(bulidIDList);
}

void SHNeighborDlg::on_m_cbxFloor_currentIndexChanged(const QString &arg1)
{
    QString floor = ui->m_cbxFloor->currentText();
    ui->m_cbxRoomID->clear();
    for(int i = 1;i<20;i++)
    {
        QString info = floor;
        if(i<10)
        {
            info.append('0');
            ui->m_cbxRoomID->addItem(info.append(i+'0'));
        }
        else
        {
            info.append('1');
            ui->m_cbxRoomID->addItem(info.append(i-10+'0'));
        }
    }
}

void SHNeighborDlg::on_m_btResearch_clicked()
{
    if(tcpClient->state() != QAbstractSocket::SocketState::ConnectedState)
        tcpClient->connectToHost("192.168.1.11",5000);
    if(tcpClient->waitForConnected(1000))
    {
        SHNetworkMessage *info = new SHNetworkMessage;
        info->setMessageType(MessageHeader_MessageType_IPINFO_REQ);
        info->setCommunity(ui->m_cbxCommunity->currentText().toStdString());
        info->setBuildingPart(ui->m_cbxBuildPart->currentText().toStdString());
        info->setBuildingID(ui->m_cbxBuildID->currentText().toStdString());
        info->setCellID(ui->m_cbxCellID->currentText().toStdString());
        info->setRoomID(ui->m_cbxRoomID->currentText().toStdString());
        info->mergeMessage();

        QByteArray data;
        info->serializeToString(data);
        tcpClient->write(data);
    }
    else
    {

    }
}

void SHNeighborDlg::readData()
{
    QByteArray buf = tcpClient->readAll();
    SHNetworkMessage *info = new SHNetworkMessage;
    info->parseMessage(buf);
    if(info->getMessageType() == MessageHeader_MessageType_IPINFO_RESP)
    {
        QString data = QString::fromStdString(info->getMessageData());
        if(data != "")
        {
            QStringList list = data.split("+");
            ui->m_txName->setText(list.at(0));
            ui->m_txIP->setText(list.at(1));
        }
        else
        {
            ui->m_txName->setText("NULL");
            ui->m_txIP->setText("NULL");
        }
    }
}

void SHNeighborDlg::on_m_btDial_clicked()
{
}
