#ifndef SHDEVICEMGRDLG_H
#define SHDEVICEMGRDLG_H

#include <QWidget>
#include <QString>
#include <QGridLayout>
#include <QListWidget>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QPainter>
#include <SH_Controls.h>

namespace Ui {
class SHDeviceMgrDlg;
}

class SHDeviceMgrDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SHDeviceMgrDlg(QWidget *parent = nullptr);
    ~SHDeviceMgrDlg();

    enum ModeOfView {
        ModeDevice,
        ModeLocate
    };

private slots:
    void on_m_btnDelete_clicked();
    void on_m_btnAlter_clicked();
    void on_m_btnBack_clicked();
    void on_m_cboxSort_currentIndexChanged(int index);

    void on_m_listwgSort_itemClicked(QListWidgetItem *item);

signals:
    void backToLaunchDlg(int dialog);

public slots:
    void deviceMgrDlgclickSlot(bool x);
    void updateScrollAreaFirst();
    void updateCursorBtn();

private:
    Ui::SHDeviceMgrDlg *ui;
    void init();
    void setNomalStyle();
    void updateListWidget();
    void updateScrollArea();
    SHToolButton *getCursorButton();

    int m_iModeOfView;        //记录查看设备方式
    QString m_szListWidgetText;  //记录当前选中的list列表项
    QString m_szCursorBtnAddr;  //记录当前拥有焦点的button的地址

    QTimer *m_timer;
    QPixmap m_pixmapBg;
    SHToolButton *m_pLastCursorBtn;

};

#endif // SHDEVICEMGRDLG_H
