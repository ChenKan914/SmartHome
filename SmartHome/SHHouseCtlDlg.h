#ifndef SHHOUSECTLDLG_H
#define SHHOUSECTLDLG_H

#include <QWidget>
#include <QListWidgetItem>
#include <SH_Controls.h>
#include <HouseCtlLamp.h>
#include <HouseCtlConditon.h>
#include <HouseCtlCurtain.h>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QFile>

namespace Ui {
class SHHouseCtlDlg;
}

class SHHouseCtlDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SHHouseCtlDlg(QWidget *parent = nullptr);
    ~SHHouseCtlDlg();

    void init();
    void setNomalStyle();
    void updateListWidgetRoom();
    void updateListWidgetDevice();
    void updateScrollAreaDeviceCtl();
    void clearScrollAreaDeviceCtl();

private slots:
    void on_m_listwgRoom_itemClicked(QListWidgetItem *item);
    void on_m_listwgDevice_itemClicked(QListWidgetItem *item);
    void on_m_btnBack_clicked();
    void updateScrollAreaFirst();

signals:
    void backToLaunchDlg(int dialog);

private:
    Ui::SHHouseCtlDlg *ui;

    QString m_szListWidgetRoomText;  //记录当前选中的list列表项
    QString m_szListWidgetDeviceText;  //记录当前选中的list列表项

    HouseCtlLamp *m_pLamp;
    HouseCtlCurtain *m_pCurtain;
    HouseCtlConditon *m_pCondition;

    QTimer *m_timer;
};

#endif // SHHOUSECTLDLG_H
