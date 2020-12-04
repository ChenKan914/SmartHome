#ifndef SHDIALOGMGRDLG_H
#define SHDIALOGMGRDLG_H

#include <QWidget>
#include <SHDeviceMgrDlg.h>
#include <SHHomeDlg.h>
#include <SHLauncherDlg.h>
#include <SHHouseCtlDlg.h>
#include <SHPropertyDlg.h>
#include <SHSecurityDlg.h>
#include <SHNeighborDlg.h>
#include <QPoint>
#include <QMouseEvent>
#include <QStackedWidget>
#include <QtCore>
#include <QPainter>
#include <QPoint>

namespace Ui {
class SHDialogMgrDlg;
}

class SHDialogMgrDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SHDialogMgrDlg(QWidget *parent = nullptr);
    ~SHDialogMgrDlg();

private slots:
    void on_m_btnHome_clicked();

public slots:
    void launchButtonClick(int index);
    void backLaunchDlg(int index);

protected:
    void paintEvent(QPaintEvent *event);        //添加背景
    void mouseMoveEvent(QMouseEvent *event);    //设置界面可拖动
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::SHDialogMgrDlg *ui;

    void init();

    SHHomeDlg *pMainDlg = new SHHomeDlg();

    SHDeviceMgrDlg *pDeviceMgrDlg;
    SHLauncherDlg *pLaunchDlg;
    SHHouseCtlDlg *pHouseCtlDlg;
    SHPropertyDlg *pPropertyDlg;
    SHSecurityDlg *pSecurityDlg;
    SHNeighborDlg *NeighborDlg;

    QPixmap m_pixmapBg;
    QPoint m_pointStart;
    QPoint m_pointPress;

};

#endif // SHDIALOGMGRDLG_H
