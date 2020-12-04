#include "SHDeviceMgrDlg.h"
#include "ui_SHDeviceMgrDlg.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QPushButton>
#include <QScrollArea>
#include <QStandardItemModel>
#include <QMessageBox>
#include "SHAddDeviceDlg.h"

SHDeviceMgrDlg::SHDeviceMgrDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SHDeviceMgrDlg),
    m_iModeOfView(ModeDevice),
    m_pLastCursorBtn(nullptr)
{
    ui->setupUi(this);
    init();
}

SHDeviceMgrDlg::~SHDeviceMgrDlg()
{
    delete ui;
}
void SHDeviceMgrDlg::init()
{
    qDebug()<<"DeviceMgrDlg Init";
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    //设置样式
    QStringList ClassifyList;
    ClassifyList<<"按房间查看"<<"按设备查看";
    ui->m_cboxSort->addItems(ClassifyList);
    ui->m_cboxSort->setAttribute(Qt::WA_NoSystemBackground,true);
    ui->m_cboxSort->setStyleSheet("combobox-popup:0");
    ui->m_lbTitle->setStyleSheet("color:#ffffff");
    setNomalStyle();

    //等待Widget控件大小初始化，初始化ScrollArea
    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(updateScrollAreaFirst()));
    m_timer->start(100);
}

void SHDeviceMgrDlg::setNomalStyle()
{
    qDebug()<<"DeviceMgrDlg setNomalStyle";
    QFile styleSheet(":/qss/style_SHDeviceMgrDlg.qss");
    if (!styleSheet.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");
    }
    else
    {
        this->setStyleSheet(styleSheet.readAll());
    }
}

void SHDeviceMgrDlg::updateScrollAreaFirst()
{
    if(ui->m_scrollArea->width() <200)
    {
        m_timer->start(100);
    }
    else
    {
        updateScrollArea();
        m_timer->stop();
    }
}

void SHDeviceMgrDlg::on_m_cboxSort_currentIndexChanged(int index)
{
    qDebug()<<"changItem";
    if(ui->m_cboxSort->currentText() == "按设备查看")
    {
        m_iModeOfView = ModeDevice;
    }
    else if(ui->m_cboxSort->currentText() == "按房间查看")
    {
        m_iModeOfView = ModeLocate;
    }
    updateListWidget();
}

void SHDeviceMgrDlg::on_m_listwgSort_itemClicked(QListWidgetItem *item)
{
    m_szListWidgetText = item->text();
    updateScrollArea();
}

void SHDeviceMgrDlg::updateListWidget()
{
    qDebug()<<"updateListWidget";
    ui->m_listwgSort->clear();
    SHStringList m_listwigdet;

    //由不同查找方式获取list列表
    QSqlQuery query;
    if(m_iModeOfView == ModeDevice)
    {
        query.exec("select Type from devicemgr");
    }
    else if(m_iModeOfView == ModeLocate)
    {
        query.exec("select Location from devicemgr");
    }
    while(query.next())
    {
        m_listwigdet<<query.value(0).toString();
    }

    m_listwigdet.removeRepeatItem();

    //刷新list列表，默认选中list第一项
    if(m_listwigdet.count() > 0)
    {
        ui->m_listwgSort->addItems(m_listwigdet);
        m_szListWidgetText = m_listwigdet.at(0);
        ui->m_listwgSort->setItemSelected(ui->m_listwgSort->item(0),true);

        updateScrollArea();
    }
}

void SHDeviceMgrDlg::updateScrollArea()
{
    qDebug()<<"DeviceMgrDlg  updateScrollArea()";

    //删除ScrollArea上的所有按钮
    QList<SHToolButton*> btns = ui->m_scrollAreaWidgetContents->findChildren<SHToolButton*>();
    foreach (SHToolButton* btn, btns) {   delete btn;  }
    //清除上一次的CursorBtn
    m_pLastCursorBtn = nullptr;

    //按list信息进行查找
    QSqlQuery query;
    if(m_iModeOfView == ModeLocate)
    {
        query.exec("select * from devicemgr where Location = '"+m_szListWidgetText+"'");
    }
    else if(m_iModeOfView == ModeDevice)
    {
        query.exec("select * from devicemgr where Type = '"+m_szListWidgetText+"'");
    }

    //由控件坐标设置位置
    int xPoint=0;
    int yPoint=0;

    while(query.next())
    {
        QString Addr = query.value(0).toString();
        QString Name = query.value(2).toString();
        QString TypeOrLocate = query.value(3).toString();

        //设置按钮坐标和大小
        //const int XLENGTH=100;

        SHToolButton *pNewDevice = new SHToolButton(ui->m_scrollAreaWidgetContents);
        pNewDevice->move(110*xPoint+15,110*yPoint+5);
        pNewDevice->resize(100,100);
        pNewDevice->show();

        //设置按钮名字和icon
        pNewDevice->setFocusPolicy(Qt::StrongFocus);
        pNewDevice->setIconSize(QSize(70,70));

        pNewDevice->setIconDeviceMgrDlg(TypeOrLocate);
        pNewDevice->setFont(QFont("Adobe Arobic",10));
        pNewDevice->setAutoRaise(true);
        pNewDevice->setStyleSheet("SHToolButton{background-color:rgba(255, 255, 255,10);color:rgb(255,255,255);}"
                                  "SHToolButton:hover{background-color:rgba(255, 255, 255,10);color:rgb(255,255,255);}"
                                  "SHToolButton:checked{background-color:rgba(255, 255, 255,10);color:rgb(255,255,255);}");
        pNewDevice->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        pNewDevice->setText(Name);

        //设置ScrollAreaWidget布局
        xPoint++;
        if(xPoint > 6)
        {
            xPoint = 0;
            yPoint++;
        }
        ui->m_scrollAreaWidgetContents->setGeometry(0,0,ui->m_scrollArea->width()-20,110*(yPoint+1)+5);

        //保存按钮信息
        pNewDevice->setBtnAddr(Addr);
        pNewDevice->setBtnName(Name);
        if(m_iModeOfView == ModeLocate)
        {
            pNewDevice->setBtnType(TypeOrLocate);
            pNewDevice->setBtnLocate(m_szListWidgetText);
        }
        else if(m_iModeOfView == ModeDevice)
        {
            pNewDevice->setBtnType(m_szListWidgetText);
            pNewDevice->setBtnLocate(TypeOrLocate);
        }

        connect(pNewDevice,SIGNAL(clicked()),this,SLOT(updateCursorBtn()));
    }
}
void SHDeviceMgrDlg::updateCursorBtn()
{
    SHToolButton *cursorbutton = getCursorButton();

    cursorbutton->setStyleSheet("background-color:rgba(0, 0, 0,30)");
    if(m_pLastCursorBtn)
    {
        m_pLastCursorBtn->setStyleSheet("background-color:rgba(255, 255, 255,20)");
    }
    m_pLastCursorBtn = cursorbutton;
}

void SHDeviceMgrDlg::on_m_btnDelete_clicked()
{
    qDebug()<<"on_m_btnDelete_clicked";
    SHToolButton *cursorbutton = getCursorButton();

    if(cursorbutton != nullptr)
    {
        m_szCursorBtnAddr= cursorbutton->getBtnAddr();

        QMessageBox::StandardButton rb = QMessageBox::warning(this, "", "你确定要删除吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            QSqlQuery query;
            query.exec("delete from devicemgr where MacAddr = '"+m_szCursorBtnAddr+"'");
            updateScrollArea();
        }
        else
        {
        }
    }
    else
    {
    }
}

void SHDeviceMgrDlg::on_m_btnAlter_clicked()
{
    qDebug()<<"on_m_btnAlter_clicked";
    SHToolButton *cursorbutton = getCursorButton();

    if(cursorbutton != nullptr)
    {
        QString Addr = cursorbutton->getBtnAddr();
        QString Name = cursorbutton->getBtnName();
        QString Type = cursorbutton->getBtnType();
        QString Locate = cursorbutton->getBtnLocate();

        SHAddDeviceDlg *pDevicedlg = new SHAddDeviceDlg(this,NULL,Addr,Name,Type,Locate);
        pDevicedlg->show();
        pDevicedlg->updateLabelTitle("修改设备信息");
        connect(pDevicedlg,SIGNAL(btnClickSignal(bool)),this,SLOT(deviceMgrDlgclickSlot(bool)));
    }
    else
    {
        return;
    }
}

void SHDeviceMgrDlg::deviceMgrDlgclickSlot(bool value)
{
    if(value == true)
    {
         updateScrollArea();
    }
}
SHToolButton *SHDeviceMgrDlg::getCursorButton()
{
    qDebug()<<"getCursorButton";
    //返回当前的拥有焦点的按钮
    SHToolButton *cursorbutton;
    QWidget *currentitem;
    currentitem = QApplication::focusWidget();
    cursorbutton = qobject_cast<SHToolButton *>(currentitem);
    return cursorbutton;
}

void SHDeviceMgrDlg::on_m_btnBack_clicked()
{
    emit backToLaunchDlg(SHEnumMgr::SH_LAUNCHDLG);
}


