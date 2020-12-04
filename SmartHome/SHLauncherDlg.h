#ifndef SHLAUNCHERDLG_H
#define SHLAUNCHERDLG_H

#include <QWidget>
#include "QToolButton"
#include "SHHomeDlg.h"
#include <SHControls.h>

namespace Ui {
class SHLauncherDlg;
}

class SHLauncherDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SHLauncherDlg(QWidget *parent = nullptr);
    ~SHLauncherDlg();

signals:
    void switchDialog(int dialog);

private slots:
    void on_m_btnDeviceMgr_clicked();
    void on_m_btnHouse_clicked();

    void on_m_btnProperty_clicked();

    void on_m_btnSafe_clicked();

    void on_m_btnNeighbor_clicked();

private:
    Ui::SHLauncherDlg *ui;

    void init();
    void setNomalStyle();
};

#endif // SHLAUNCHERDLG_H
