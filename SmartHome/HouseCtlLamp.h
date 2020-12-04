#ifndef HOUSECTLLAMP_H
#define HOUSECTLLAMP_H

#include <QWidget>
#include <QString>

namespace Ui {
class HouseCtlLamp;
}

class HouseCtlLamp : public QWidget
{
    Q_OBJECT

public:
    explicit HouseCtlLamp(QWidget *parent = nullptr,QString Addr = nullptr,QString ShortAddr = nullptr,QString Name = nullptr);
    ~HouseCtlLamp();

    void init();

private:
    Ui::HouseCtlLamp *ui;

    QString m_szName;
    QString m_szAddr;
    QString m_szShortAddr;

};

#endif // HOUSECTLLAMP_H
