#ifndef SECURITYHUMITURE_H
#define SECURITYHUMITURE_H

#include <QWidget>
#include "SHSerialPort.h"
#include <QMessageBox>
namespace Ui {
class SecurityHumiture;
}

class SecurityHumiture : public QWidget
{
    Q_OBJECT

public:
    explicit SecurityHumiture(QWidget *parent = nullptr,QString type = "");
    ~SecurityHumiture();

    void init(QString type);
    void setDeviceName(QString name);

private slots:
    void updateHumitureData(QString temp,QString humi);
    void updateFireAlarm();

private:
    Ui::SecurityHumiture *ui;
};

#endif // SECURITYHUMITURE_H
