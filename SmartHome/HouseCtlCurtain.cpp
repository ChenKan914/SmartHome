#include "HouseCtlCurtain.h"
#include "ui_HouseCtlCurtain.h"

HouseCtlCurtain::HouseCtlCurtain(QWidget *parent,QString Addr,QString ShortAddr,QString Name) :
    QWidget(parent),
    ui(new Ui::HouseCtlCurtain)
{
    ui->setupUi(this);

    m_szAddr = Addr;
    m_szName = Name;
    m_szShortAddr = ShortAddr;
    init();
}

HouseCtlCurtain::~HouseCtlCurtain()
{
    delete ui;
}

void HouseCtlCurtain::init()
{
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("background-color:rgb(255,255,255,20)");

    ui->m_lbName->setText(m_szName);
    ui->m_lbName->setAlignment(Qt::AlignVCenter);
    ui->m_lbName->setStyleSheet("background-color:rgb(0,0,0,0)");

    ui->m_btnOpen->setIconSize(QSize(50,50));
    ui->m_btnOpen->setIcon(QIcon(QPixmap(":/images/HouseCtlDlg/CurtainOpen.png")));
    ui->m_btnClose->setIconSize(QSize(50,50));
    ui->m_btnClose->setIcon(QIcon(QPixmap(":/images/HouseCtlDlg/CurtainClose.png")));
    ui->m_btnPause->setIconSize(QSize(50,50));
    ui->m_btnPause->setIcon(QIcon(QPixmap(":/images/HouseCtlDlg/CurtainPause.png")));
}
