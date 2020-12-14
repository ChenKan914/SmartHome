#include "SHLauncherDlg.h"
#include "ui_SHLauncherDlg.h"
#include <QPainter>
#include <QFile>
#include <SH_Controls.h>

SHLauncherDlg::SHLauncherDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SHLauncherDlg)
{
    ui->setupUi(this);
    init();
}

SHLauncherDlg::~SHLauncherDlg()
{
    delete ui;
}

void SHLauncherDlg::init()
{
    //设置无边框，背景透明
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setNomalStyle();
}

void SHLauncherDlg::setNomalStyle()
{
    QFile styleSheet(":/qss/style_SHLauncherDlg.qss");
    if (!styleSheet.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");
    }
    else
    {
        this->setStyleSheet(styleSheet.readAll());
    }
}

void SHLauncherDlg::on_m_btnDeviceMgr_clicked()
{
    emit switchDialog(SHEnumMgr::SH_DEVICEMGRDLG);
}

void SHLauncherDlg::on_m_btnHouse_clicked()
{
    emit switchDialog(SHEnumMgr::SH_HOUSECTLDLG);
}

void SHLauncherDlg::on_m_btnProperty_clicked()
{
    emit switchDialog(SHEnumMgr::SH_PROPERTYDLG);
}

void SHLauncherDlg::on_m_btnSafe_clicked()
{
    emit switchDialog(SHEnumMgr::SH_SECURITYDLG);
}

void SHLauncherDlg::on_m_btnNeighbor_clicked()
{
    emit switchDialog(SHEnumMgr::SH_NEIGHBORDLG);
}
