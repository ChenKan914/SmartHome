#ifndef SH_CONTROLSH
#define SH_CONTROLSH

#include "sh_controls_global.h"
#include <QToolButton>
#include <QStringList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QtDebug>
class SHToolButton : public QToolButton
{
    Q_OBJECT

public:
    explicit SHToolButton(QWidget *parent = nullptr);
    ~SHToolButton() {}


    void setBtnAddr(const QString &text)    {m_btnAddr = text;}
    void setBtnName(const QString &text)    {m_btnName = text;}
    void setBtnType(const QString &text)    {m_btnType = text;}
    void setBtnLocate(const QString &text)  {m_btnLocate = text;}

    QString getBtnAddr()    {return m_btnAddr;}
    QString getBtnName()    {return m_btnName;}
    QString getBtnType()    {return m_btnType;}
    QString getBtnLocate()  {return m_btnLocate;}

    void setIconDeviceMgrDlg(QString Type);


private:
    QString m_btnAddr;
    QString m_btnName;
    QString m_btnType;
    QString m_btnLocate;
};

class SHStringList : public QStringList
{
public:
    explicit SHStringList() {}
    ~SHStringList() {}

    SHStringList *removeRepeatItem();  //去除QStringlist重复项
};

class SHListWidgetItem :public QListWidgetItem
{
public:
    enum ItemType { Type = 0, UserType = 1000 };
    explicit SHListWidgetItem(QListWidget *view = nullptr, int type = Type);
    virtual ~SHListWidgetItem();

    void setAddr(const QString &text)    {m_btnAddr = text;}
    void setType(const QString &text)    {m_btnType = text;}

    QString getAddr()    {return m_btnAddr;}
    QString getType()    {return m_btnType;}

private:
    QString m_btnAddr;
    QString m_btnType;
};

class SHEnumMgr{
public:
    enum DialogSwitch{
        SH_SYSTEMDLG1,
        SH_SYSTEMDLG2,
        SH_MAINDLG,
        SH_LAUNCHDLG,
        SH_DEVICEMGRDLG,
        SH_HOUSECTLDLG,
        SH_PROPERTYDLG,
        SH_SECURITYDLG,
        SH_NEIGHBORDLG
    };
};

typedef enum
{
    SMARTHOME_MACADDRMSG = 0,
    SMARTHOME_TEMPHUMIMSG = 1,
    SMARTHOME_AIRCONDITIONMSG = 2,
    SMARTHOME_FIREALARM = 3
} msgType_t;


typedef struct
{
    uint8_t msgType;
    uint8_t len;
    uint8_t data[10];
}uartMsgBody;

typedef struct
{
    uint8_t header = 0xff;
    uint16_t shortAddr = 0x0000;
    uartMsgBody body;
    uint8_t tailer = 0x11;
}uartSendMsg;

typedef struct
{
    QString shortAddr;
    QString extAddr;
    int msgType;
    int lenth;
    QString data;
}changeMsg;
#endif // SH_CONTROLSH
