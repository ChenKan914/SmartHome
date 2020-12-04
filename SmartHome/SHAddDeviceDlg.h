#ifndef SHADDDEVICEDLG_H
#define SHADDDEVICEDLG_H

#include <QDialog>
#include <QString>

namespace Ui {
class SHAddDeviceDlg;
}

class SHAddDeviceDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SHAddDeviceDlg(QWidget *parent = 0,QString ShortAddr = NULL,QString DeviceAddr = NULL,QString DeviceName = NULL,QString DeviceType = NULL,QString DeviceLocate = NULL);
    ~SHAddDeviceDlg();


    void updateLabelTitle(QString label);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::SHAddDeviceDlg *ui;

    void init(QString Addr,QString Name,QString Type,QString Locate);

    QString m_shortAddr;
signals:
    void btnClickSignal(bool x);
};

#endif // SHADDDEVICEDLG_H
