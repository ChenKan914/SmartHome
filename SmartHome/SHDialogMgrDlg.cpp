#include "SHDialogMgrDlg.h"
#include "ui_SHDialogMgrDlg.h"

SHDialogMgrDlg::SHDialogMgrDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SHDialogMgrDlg)
{
    ui->setupUi(this);
    init();
}

SHDialogMgrDlg::~SHDialogMgrDlg()
{
    delete ui;
}

void SHDialogMgrDlg::init()
{
    //设置无边框，背景透明
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    //添加背景，设置样式
    m_pixmapBg.load(":/images/Background/bg2.jpg");
    int iWidth = m_pixmapBg.width(); //获得以前图片的宽和高
    int iWeight = m_pixmapBg.height();
    m_pixmapBg = m_pixmapBg.scaled(iWidth/2,iWeight/2,Qt::KeepAspectRatio);
    ui->m_wgTool->setStyleSheet("background-color: rgba(0, 0, 0,30 )");

    //界面管理
    pMainDlg = new SHHomeDlg;
    pLaunchDlg = new SHLauncherDlg;
    pDeviceMgrDlg = new SHDeviceMgrDlg;
    pHouseCtlDlg = new SHHouseCtlDlg;
    pPropertyDlg = new SHPropertyDlg;
    pSecurityDlg = new SHSecurityDlg;
    NeighborDlg = new SHNeighborDlg;

    connect(pLaunchDlg,SIGNAL(switchDialog(int)),this,SLOT(launchButtonClick(int)));
    connect(pDeviceMgrDlg,SIGNAL(backToLaunchDlg(int)),this,SLOT(backLaunchDlg(int)));
    connect(pHouseCtlDlg,SIGNAL(backToLaunchDlg(int)),this,SLOT(backLaunchDlg(int)));
    connect(pPropertyDlg,SIGNAL(backToLaunchDlg(int)),this,SLOT(backLaunchDlg(int)));
    connect(pSecurityDlg,SIGNAL(backToLaunchDlg(int)),this,SLOT(backLaunchDlg(int)));
    connect(NeighborDlg,SIGNAL(backToLaunchDlg(int)),this,SLOT(backLaunchDlg(int)));

    ui->m_stackwgMain->addWidget(pMainDlg);
    ui->m_stackwgMain->addWidget(pLaunchDlg);
    ui->m_stackwgMain->addWidget(pDeviceMgrDlg);
    ui->m_stackwgMain->addWidget(pHouseCtlDlg);
    ui->m_stackwgMain->addWidget(pPropertyDlg);
    ui->m_stackwgMain->addWidget(pSecurityDlg);
    ui->m_stackwgMain->addWidget(NeighborDlg);

    ui->m_stackwgMain->setCurrentIndex(SHEnumMgr::SH_MAINDLG);
}

void SHDialogMgrDlg::launchButtonClick(int index)
{
     ui->m_stackwgMain->setCurrentIndex(index);
}

void SHDialogMgrDlg::backLaunchDlg(int index)
{
    ui->m_stackwgMain->setCurrentIndex(index);
}

void SHDialogMgrDlg::on_m_btnHome_clicked()
{
    if(ui->m_stackwgMain->currentIndex() == SHEnumMgr::SH_MAINDLG)
    {
        ui->m_stackwgMain->setCurrentIndex(SHEnumMgr::SH_LAUNCHDLG);
        QIcon button_ico(":/images/Toolbar/home.png");
        ui->m_btnHome->setIcon(button_ico);
    }
    else
    {
        ui->m_stackwgMain->setCurrentIndex(SHEnumMgr::SH_MAINDLG);
        QIcon button_ico(":/images/Toolbar/app.png");
        ui->m_btnHome->setIcon(button_ico);
    }
}

void SHDialogMgrDlg::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(m_pixmapBg.rect(), m_pixmapBg);
}

void SHDialogMgrDlg::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - m_pointStart);
}

void SHDialogMgrDlg::mousePressEvent(QMouseEvent *event)
{
    m_pointPress = event->globalPos();
    m_pointStart = m_pointPress - this->pos();
}
