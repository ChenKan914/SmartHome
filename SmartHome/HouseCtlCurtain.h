#ifndef HOUSECTLCURTAIN_H
#define HOUSECTLCURTAIN_H

#include <QWidget>

namespace Ui {
class HouseCtlCurtain;
}

class HouseCtlCurtain : public QWidget
{
    Q_OBJECT

public:
    explicit HouseCtlCurtain(QWidget *parent = nullptr,QString Addr = nullptr,QString ShortAddr = nullptr,QString Name = nullptr);
    ~HouseCtlCurtain();

    void init();

private:
    Ui::HouseCtlCurtain *ui;

    QString m_szName;
    QString m_szAddr;
    QString m_szShortAddr;
};

#endif // HOUSECTLCURTAIN_H
