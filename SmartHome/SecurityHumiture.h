#ifndef SECURITYHUMITURE_H
#define SECURITYHUMITURE_H

#include <QWidget>
#include "SHSerialPort.h"
extern SHSerialPort *pSHSerial;
namespace Ui {
class SecurityHumiture;
}

class SecurityHumiture : public QWidget
{
    Q_OBJECT

public:
    explicit SecurityHumiture(QWidget *parent = nullptr);
    ~SecurityHumiture();

    void init();
    void setDeviceName(QString name);

private slots:
    void updateHumitureData(QString temp,QString humi);

private:
    Ui::SecurityHumiture *ui;
};

#endif // SECURITYHUMITURE_H
