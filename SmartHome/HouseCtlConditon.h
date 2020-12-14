 #ifndef HOUSECTLCONDITON_H
#define HOUSECTLCONDITON_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>
#include <SH_Controls.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

namespace Ui {
class HouseCtlConditon;
}

class HouseCtlConditon : public QWidget
{
    Q_OBJECT

public:
    explicit HouseCtlConditon(QWidget *parent = nullptr,QString Addr = nullptr,QString ShortAddr = nullptr,QString Name = nullptr);
    ~HouseCtlConditon();

    void init();

    //
    void sendUartMsg(QString commandFront,QString commandRear);
    void stringReplace(QString &origin,QString object,int n);
    void generateCheckNum(QString &checkNumArray);
    void updateInterface(QString commandFront,QString commandRear);

    //更新UI界面
    void runModeSelect(int mode);
    void tempShowModeSelect(int tempShow);
    void windSpeedSelect(int windSpeed);
    void upDownSwingModeSelect(int upDownSwing);
    void upDownSwingAllShow(bool isShow);
    void leftRightSwingModeSelect(int leftRightSwing);
    void leftRightSwingAllShow(bool isShow);

    //更新模式码和数据库
    void updateModeCode();
    void updateAllFrontCode();
    void updateAllRearCode();

private slots:

    void on_m_btnSwitch_clicked();              //开关
    void on_m_btnMode_clicked();                //模式
    void on_m_btnAdd_clicked();                 //+
    void on_m_btnSub_clicked();                 //-
    void on_m_btnWindSpeed_clicked();           //风速
    void on_m_btnLight_clicked();               //灯光
    void on_m_btnStrong_clicked();              //超强
    void on_m_btnSleep_clicked();               //睡眠
    void on_m_btnTempShow_clicked();            //温度显示
    void on_m_btnUpDownSwing_clicked();         //上下扫风
    void on_m_btnLeftRightSwing_clicked();      //左右扫风
    void on_m_btnDryAuxHeat_clicked();          //干燥辅热
    void on_m_btnSaveEnergy_clicked();          //节能
    void on_m_btnHealthSwap_clicked();          //健康换气
    void on_m_btnTimer_clicked();               //定时

    void isStartBootTimer();
    void isStartRunTimer();

private:
    Ui::HouseCtlConditon *ui;

    QString m_szName;
    QString m_szAddr;
    QString m_szShortAddr;

    QString audoMode1;
    QString coldMode1;
    QString wetMode1;
    QString windMode1;
    QString hotMode1;

    QString audoMode2;
    QString coldMode2;
    QString wetMode2;
    QString windMode2;
    QString hotMode2;

    QString operateCode1;
    QString operateCode2;

    int m_currentTmp;                   //温度
    int m_currentRunMode;               //模式
    int m_currentWindSpeed;             //风速
    bool m_currentStrong;               //超强
    bool m_currentLight;                //灯光
    bool m_currentSleep;                //睡眠
    int m_currentTempShowMode;          //温度显示模式
    int m_currentUpDownSwingMode;       //上下扫风模式
    int m_currentLeftRightSwingMode;    //左右扫风模式
    int m_currentLeftRightSwingValue;   //左右扫风值
    bool m_currentSwapAir;              //换气
    double m_currentTimers;             //定时器时间
    int m_timerBootTimes;               //定时次数
    int m_timerRunTimes;                //定时次数

    QTimer *m_timerBoot;
    QTimer *m_timerRun;


public:

    QString checkNumArray = "0000";

    QString runMode[5] = {"000","100","010","110","001"};

    QString windSpeed[4] = {"00","10","01","11"};

    QString temperature[15] =
    {
            "0000","1000","0100","1100","0010",
            "1010","0110","1110","0001","1001",
            "0101","1101","0011","1011","0111"
    };

    QString tempShowMode[4] = {"00","10","01","11"};

    QString upDownSwing[10] =
    {
        "0000","1000","0100","1100","0010",
        "1010","0110","1110","1001","1101"
    };

    QString leftRightSwing[9] =
    {
        "0000","1000","0100","1100","0010",
        "1010","0110","0011","1011"
    };

    QString timerData[3] = {"001","101","011"};         //定时器低位编码

    int leftRightSwingValue[9] ={0,1,2,3,4,5,6,12,13};

    enum LEFT_RIGHT_SWING
    {
        LEFTRIGHT_NONE_SWING = 0,
        LEFTRIGHT_ALL_FIVE_SWING = 1,
        LEFTRIGHT_LEFT_LEFT_SWING = 2,
        LEFTRIGHT_LEFT_SWING,
        LEFTRIGHT_MID_SWING,
        LEFTRIGHT_RIGHT_SWING,
        LEFTRIGHT_RIGHT_RIGHT_SWING,
        LEFTRIGHT_RIGHT_TWO_SWING = 7,
        LEFTRIGHT_SWING
    };

    enum UP_DOWN_SWING
    {
        UPDOWN_NONE_SWING = 0,
        UPDOWN_ALL_FIVE_SWING = 1,
        UPDOWN_UP_UP_SWING = 2,
        UPDOWN_UP_SWING,
        UPDOWN_MID_SWING,
        UPDOWN_DOWN_SWING,
        UPDOWN_DOWN_DOWN_SWING,
        UPDOWN_DOWN_THREE_SWING = 7,
        UPDOWN_MID_THREE_SWING,
        UPDOWN_UP_THREE_SWING
    };

    enum TEMP_SHOW_MODE
    {
        NONE = 0,
        SET  = 1,
        INDOOR = 2,
        OUTDOOR = 3
    };

    enum RUN_MODE
    {
        AUTO = 0,
        COLD = 1,
        WET = 2,
        WIND = 3,
        HOT =4
    };

    enum WIND_SPEED
    {
        AUTO_LEVEL = 0,
        FIRST_LEVEL = 1,
        SECOND_LEVEL = 2,
        THIRD_LEVEL = 3
    };

    struct CODE35{
        char mode[3];
        char onOff;
        char windSpeed[2];
        char sweeping;
        char sleeping;
        char temperature[4];
        char timer[8];
        char humidification;
        char lightOn;
        char anion;
        char powerSaving;
        char switchAir;
        char data[10];
    };

    struct CODE32{
            char upDownSweeping;
            char data1[3];
            char leftRightSweeping;
            char data2[3];
            char showTemp[2];
            char data3[16];
            char saveEnerge;
            char data4;
            char checkCode[4];
    };
};

#endif // HOUSECTLCONDITON_H
