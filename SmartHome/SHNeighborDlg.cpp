#include "SHNeighborDlg.h"
#include "ui_SHNeighborDlg.h"
#include <QFile>
#include<QProcess>

SHNeighborDlg::SHNeighborDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SHNeighborDlg),
    insertTableFromSelect(-1)
{
    setNomalStyle();
    ui->setupUi(this);

    tcpClient = new QTcpSocket;
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(readData()));
    on_m_cbxBuildPart_currentTextChanged("");
    updateTableWidget();
    ui->m_tbwgHistory->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void SHNeighborDlg::updateTableWidget()
{
    QSqlQuery query;
    query.exec("select * from dialHistory");
    while(query.next())
    {
        ui->m_tbwgHistory->insertRow(0);
        ui->m_tbwgHistory->setItem(0,0,new QTableWidgetItem(query.value(0).toString()));
        ui->m_tbwgHistory->setItem(0,1,new QTableWidgetItem(query.value(1).toString()));
        ui->m_tbwgHistory->setItem(0,2,new QTableWidgetItem(query.value(2).toString()));
        ui->m_tbwgHistory->setItem(0,3,new QTableWidgetItem(query.value(3).toString()));
        ui->m_tbwgHistory->setItem(0,4,new QTableWidgetItem(query.value(4).toString()));
        ui->m_tbwgHistory->setItem(0,5,new QTableWidgetItem(query.value(5).toString()));
    }
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
    insertTableFromSelect = -1;
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
            ui->m_txName->setText("");
            ui->m_txIP->setText("");
            QMessageBox::warning(this, "查询结果", "未查询到该房间的信息。", QMessageBox::Yes);
        }
    }
}

void SHNeighborDlg::on_m_btDial_clicked()
{
    if(ui->m_txName->toPlainText() == "")
    {
        QMessageBox::warning(this, "错误", "未选择联系人。", QMessageBox::Yes);
    }
    else if(insertTableFromSelect == -1)
    {
        QSqlQuery query;
        query.exec("insert into dialHistory(BuildingPart,BuildingID,CellID,RoomID,Name,Time,IP) values("
                   "'"+ui->m_cbxBuildPart->currentText()+"',"
                   "'"+ui->m_cbxBuildID->currentText()+"',"
                   "'"+ui->m_cbxCellID->currentText()+"',"
                   "'"+ui->m_cbxRoomID->currentText()+"',"
                   "'"+ui->m_txName->toPlainText()+"',"
                   "'"+QDate::currentDate().toString("MM-dd")+" "+QTime::currentTime().toString("hh:mm")+"',"
                   "'"+ui->m_txIP->toPlainText()+"')");

        ui->m_tbwgHistory->insertRow(0);
        ui->m_tbwgHistory->setItem(0,0,new QTableWidgetItem(ui->m_cbxBuildPart->currentText()));
        ui->m_tbwgHistory->setItem(0,1,new QTableWidgetItem(ui->m_cbxBuildID->currentText()));
        ui->m_tbwgHistory->setItem(0,2,new QTableWidgetItem(ui->m_cbxCellID->currentText()));
        ui->m_tbwgHistory->setItem(0,3,new QTableWidgetItem(ui->m_cbxRoomID->currentText()));
        ui->m_tbwgHistory->setItem(0,4,new QTableWidgetItem(ui->m_txName->toPlainText()));
        ui->m_tbwgHistory->setItem(0,5,new QTableWidgetItem(QDate::currentDate().toString("MM-dd")+" "+QTime::currentTime().toString("hh:mm")));
    }
    else
    {
        QSqlQuery query;
        query.exec("select BuildingPart,BuildingID,CellID,RoomID from dialHistory where IP = '"+ui->m_txIP->toPlainText()+"' ");
        while(query.next())
        {
            QSqlQuery query1;
            query1.exec("insert into dialHistory(BuildingPart,BuildingID,CellID,RoomID,Name,Time,IP) values("
                       "'"+query.value(0).toString()+"',"
                       "'"+query.value(1).toString()+"',"
                       "'"+query.value(2).toString()+"',"
                       "'"+query.value(3).toString()+"',"
                       "'"+ui->m_txName->toPlainText()+"',"
                       "'"+QDate::currentDate().toString("MM-dd")+" "+QTime::currentTime().toString("hh:mm")+"',"
                       "'"+ui->m_txIP->toPlainText()+"')");
            break;
        }
        ui->m_tbwgHistory->insertRow(0);
        ui->m_tbwgHistory->setItem(0,0,new QTableWidgetItem(ui->m_tbwgHistory->item(ui->m_tbwgHistory->currentRow(),0)->text()));
        ui->m_tbwgHistory->setItem(0,1,new QTableWidgetItem(ui->m_tbwgHistory->item(ui->m_tbwgHistory->currentRow(),1)->text()));
        ui->m_tbwgHistory->setItem(0,2,new QTableWidgetItem(ui->m_tbwgHistory->item(ui->m_tbwgHistory->currentRow(),2)->text()));
        ui->m_tbwgHistory->setItem(0,3,new QTableWidgetItem(ui->m_tbwgHistory->item(ui->m_tbwgHistory->currentRow(),3)->text()));
        ui->m_tbwgHistory->setItem(0,4,new QTableWidgetItem(ui->m_txName->toPlainText()));
        ui->m_tbwgHistory->setItem(0,5,new QTableWidgetItem(QDate::currentDate().toString("MM-dd")+" "+QTime::currentTime().toString("hh:mm")));
    }
}

void SHNeighborDlg::on_m_tbwgHistory_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    insertTableFromSelect = current->row();
    qDebug()<<"on_m_tbwgHistory_currentItemChanged";
    QString buildPart = ui->m_tbwgHistory->item(current->row(),0)->text();
    QString buildID = ui->m_tbwgHistory->item(current->row(),1)->text();
    QString cellID = ui->m_tbwgHistory->item(current->row(),2)->text();
    QString roomID = ui->m_tbwgHistory->item(current->row(),3)->text();

    QSqlQuery query;
    query.exec("select Name,IP from dialHistory where BuildingPart = '"+buildPart+"' and "
                                                 "BuildingID = '"+buildID+"' and "
                                                 "CellID = '"+cellID+"' and "
                                                 "RoomID = '"+roomID+"'");
    while(query.next())
    {
        ui->m_txName->setText(query.value("Name").toString());
        ui->m_txIP->setText(query.value("IP").toString());
        qDebug()<<query.value("Name").toString()<<" "<<query.value("IP").toString()<<" query";
        break;
    }
}
