#include "HouseCtlConditon.h"
#include "ui_HouseCtlConditon.h"
#include "SHSerialPort.h"
#include <QSqlDatabase>
#include <QSqlQuery>

HouseCtlConditon::HouseCtlConditon(QWidget *parent,QString Addr,QString ShortAddr,QString Name) :
    QWidget(parent),
    ui(new Ui::HouseCtlConditon),
    m_currentTmp(25),
    m_currentRunMode(AUTO),
    m_currentWindSpeed(AUTO_LEVEL),
    m_currentStrong(0),
    m_currentLight(1),
    m_currentSleep(0),
    m_currentTempShowMode(0),
    m_currentUpDownSwingMode(0),
    m_currentLeftRightSwingMode(0),
    m_currentLeftRightSwingValue(0),
    m_currentSwapAir(0),
    m_currentTimers(0.5),
    m_timerBootTimes(0),
    m_timerRunTimes(0)
{
    ui->setupUi(this);

    m_szAddr = Addr;
    m_szName = Name;
    m_szShortAddr = ShortAddr;

    m_timerBoot = new QTimer();
    m_timerRun = new QTimer();
    connect(m_timerBoot,SIGNAL(timeout()),this,SLOT(isStartBootTimer()));
    connect(m_timerRun,SIGNAL(timeout()),this,SLOT(isStartRunTimer()));
    init();
}

HouseCtlConditon::~HouseCtlConditon()
{
    qDebug()<<"~HouseCtlConditon";
    delete ui;
}

void HouseCtlConditon::init()
{
    setAttribute(Qt::WA_StyledBackground);

    QFile styleSheet(":/qss/style_HouseCtlCondition.qss");

    if (!styleSheet.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");
    }
    else
    {
        this->setStyleSheet(styleSheet.readAll());
    }

    QSqlQuery query;
    query.exec("select * from aircondition where MacAddr = '"+m_szAddr+"'");
    if(query.next() != NULL)
    {
        audoMode1 = query.value(1).toString();
        audoMode2 = query.value(2).toString();
        coldMode1 = query.value(3).toString();
        coldMode2 = query.value(4).toString();
        wetMode1 =  query.value(5).toString();
        wetMode2 =  query.value(6).toString();
        windMode1 = query.value(7).toString();
        windMode2 = query.value(8).toString();
        hotMode1 =  query.value(9).toString();
        hotMode2 =  query.value(10).toString();

        operateCode1 = query.value(11).toString();
        operateCode2 = query.value(12).toString();
    }

    updateInterface(operateCode1,operateCode2);
}

void HouseCtlConditon::updateInterface(QString commandFront,QString commandRear)
{
    //运行模式选择
    QString modes = commandFront.mid(0,3);
    for(int i=0;i<5;i++)
    {
        if( modes == runMode[i])
        {
            m_currentRunMode = i;
        }
    }
    runModeSelect(m_currentRunMode);

    if(m_currentRunMode == HOT)
    {
        ui->m_lbAuxHeat->show();
    }
    else
    {
        ui->m_lbAuxHeat->hide();
    }

    //温度数据显示
    QString temp = commandFront.mid(8,4);
    for(int i=0;i<15;i++)
    {
        if( temp == temperature[i])
        {
            int tem = 16 + i;
            m_currentTmp = tem;
            ui->m_lbTemp->setText(QString::number(m_currentTmp));
        }
    }

    //风速
    QString windSpeeds = commandFront.mid(4,2);
    for(int i=0;i<4;i++)
    {
        if( windSpeeds == windSpeed[i])
        {
            m_currentWindSpeed = i;
        }
    }
    windSpeedSelect(m_currentWindSpeed);

    //灯光
    if(commandFront[21] == '1')
    {
        ui->m_btnLightIcon->show();
        m_currentLight = 1;
    }
    else
    {
        ui->m_btnLightIcon->hide();
        m_currentLight = 0;
    }

    //超强
    if(commandFront[20] == '1')
    {
        ui->m_lbStrong->show();
        m_currentStrong = 1;
    }
    else
    {
        ui->m_lbStrong->hide();
        m_currentStrong = 0;
    }

    //睡眠
    if(commandFront[7] == '1')
    {
        ui->m_btnSleepIcon->show();
        m_currentSleep = 1;
    }
    else
    {
        ui->m_btnSleepIcon->hide();
        m_currentSleep = 0;
    }

    //温度显示模式
    QString tempshows = commandRear.mid(8,2);
    for(int i=0;i<4;i++)
    {
        if( tempshows == tempShowMode[i])
        {
            m_currentTempShowMode = i;
        }
    }
    tempShowModeSelect(m_currentTempShowMode);

    //上下扫风
    QString upDownSwings = commandRear.mid(0,4);
    for(int i=0;i<10;i++)
    {
        if( upDownSwings == upDownSwing[i])
        {
            m_currentUpDownSwingMode = i;
        }
    }
    upDownSwingModeSelect(m_currentUpDownSwingMode);

    //左右扫风
    QString leftRightSwings = commandRear.mid(4,4);
    for(int i=0;i<9;i++)
    {
        if( leftRightSwings == leftRightSwing[i])
        {
            m_currentLeftRightSwingMode = i;
            m_currentLeftRightSwingValue = leftRightSwingValue[i];
        }
    }
    leftRightSwingModeSelect(m_currentLeftRightSwingMode);

    //干燥辅热
    if(commandFront[23] == '1')
    {
        if(m_currentRunMode == HOT)
        {
            ui->m_lbAuxHeatOff->show();
        }
        else if((m_currentRunMode == COLD)||(m_currentRunMode == WET))
        {
            ui->m_lbDry->show();
        }
        else
        {
            ui->m_lbAuxHeatOff->hide();
            ui->m_lbDry->hide();
        }
    }
    else
    {
        if(m_currentRunMode == HOT)
        {
            ui->m_lbAuxHeatOff->hide();
        }
        else if((m_currentRunMode == COLD)||(m_currentRunMode == WET))
        {
            ui->m_lbDry->hide();
        }
        else
        {
            ui->m_lbAuxHeatOff->hide();
            ui->m_lbDry->hide();
        }
    }

    //节能
    if((m_currentRunMode == COLD)&&(commandRear[26] == '1'))
    {
        ui->m_lbTemp->hide();
        ui->m_lbMarkTemp->hide();
        ui->m_lbSaveEnergy->show();
    }
    else
    {
        ui->m_lbSaveEnergy->hide();
    }

    //健康换气
    if(commandFront[22] == '1')
    {
        ui->m_lbHealth->show();
    }
    else
    {
        ui->m_lbHealth->hide();
    }

    if(commandFront[24] == '1')
    {
        ui->m_lbSwapAir->show();
        m_currentSwapAir = 1;
    }
    else
    {
        ui->m_lbSwapAir->hide();
        m_currentSwapAir = 0;
    }

    //定时数据
    QString timerDatas = commandFront.mid(12,8);
    double m_currentTimer;
    if(timerDatas[0] == '1')
    {
        m_currentTimer = 0.5;
    }
    else
    {
        m_currentTimer = 0;
    }

    for(int i = 0 ;i<3 ;i++)
    {
        if(timerDatas.mid(1,3) == timerData[i])
        {
            m_currentTimer = m_currentTimer + 10*i;
        }
    }

    for(int i = 0 ;i<10 ;i++)
    {
        if(timerDatas.mid(4,7) == temperature[i])
        {
            m_currentTimer = m_currentTimer + i;
        }
    }

    if(m_currentTimer == 0.0)
    {
        ui->m_lbTimer->hide();
        ui->m_lbTimer2->hide();
    }
    else
    {
        ui->m_lbTimer->show();
        ui->m_lbTimer2->show();
        ui->m_lbTimer->setText(QString::number(m_currentTimer));
    }

    //开关显示
    if(commandFront[3] == '1')
    {
        ui->m_lbSwitch->show();
        ui->m_lbWindSpeed->show();
        ui->m_WindSpeed->show();


        if((m_currentRunMode == AUTO)||((m_currentRunMode == COLD)&&(commandRear[26] == '1')))
        {
            ui->m_lbTemp->hide();
            ui->m_lbMarkTemp->hide();
        }
        else
        {
            ui->m_lbTemp->show();
            ui->m_lbMarkTemp->show();
        }
    }
    else
    {
        ui->m_lbSwitch->hide();
        //隐藏所有模式图标
        runModeSelect(5);
        ui->m_lbWindSpeed->hide();
        ui->m_WindSpeed->hide();

        ui->m_lbTemp->show();
        ui->m_lbMarkTemp->show();
        ui->m_btnTempSetIcon->hide();
        ui->m_btnTempIndoorIcon->hide();
        ui->m_btnTempOutdoorIcon->hide();
        ui->m_lbAuxHeat->hide();
        ui->m_lbSaveEnergy->hide();
        ui->m_lbSwapAir->hide();
        ui->m_lbHealth->hide();

        upDownSwingAllShow(false);
        leftRightSwingAllShow(false);
    }
}

void HouseCtlConditon::runModeSelect(int mode)
{
    ui->m_btnAuto->hide();
    ui->m_btnCold->hide();
    ui->m_btnWet->hide();
    ui->m_btnWind->hide();
    ui->m_btnHot->hide();

    switch (mode)
    {
        case AUTO: ui->m_btnAuto->show();break;
        case COLD: ui->m_btnCold->show();break;
        case WET: ui->m_btnWet->show();break;
        case WIND: ui->m_btnWind->show();break;
        case HOT: ui->m_btnHot->show();break;
    default:break;
    }
}

void HouseCtlConditon::upDownSwingModeSelect( int upDownSwingMode)
{
    switch (upDownSwingMode)
    {
        case UPDOWN_NONE_SWING:
                upDownSwingAllShow(false);
                break;
        case UPDOWN_ALL_FIVE_SWING:
                upDownSwingAllShow(true);
                break;
        case UPDOWN_UP_UP_SWING:
                upDownSwingAllShow(false);
                ui->m_btnUpDownSwing1->show();
                break;
        case UPDOWN_UP_SWING:
                upDownSwingAllShow(false);
                ui->m_btnUpDownSwing2->show();
                break;
        case UPDOWN_MID_SWING:
                upDownSwingAllShow(false);
                ui->m_btnUpDownSwing3->show();
                break;
        case UPDOWN_DOWN_SWING:
                upDownSwingAllShow(false);
                ui->m_btnUpDownSwing4->show();
                break;
        case UPDOWN_DOWN_DOWN_SWING:
                upDownSwingAllShow(false);
                ui->m_btnUpDownSwing5->show();
                break;
        case UPDOWN_DOWN_THREE_SWING:
                upDownSwingAllShow(true);
                ui->m_btnUpDownSwing1->hide();
                ui->m_btnUpDownSwing2->hide();
                break;
        case UPDOWN_MID_THREE_SWING:
                upDownSwingAllShow(true);
                ui->m_btnUpDownSwing1->hide();
                ui->m_btnUpDownSwing5->hide();
                break;
        case UPDOWN_UP_THREE_SWING:
                upDownSwingAllShow(true);
                ui->m_btnUpDownSwing4->hide();
                ui->m_btnUpDownSwing5->hide();
                break;
    default:break;
    }
}

void HouseCtlConditon::leftRightSwingModeSelect( int leftRightSwing)
{
    switch (leftRightSwing)
    {
        case LEFTRIGHT_NONE_SWING:
                leftRightSwingAllShow(false);
                break;
        case LEFTRIGHT_ALL_FIVE_SWING:
                leftRightSwingAllShow(true);
                break;
        case LEFTRIGHT_LEFT_LEFT_SWING:
                leftRightSwingAllShow(false);
                ui->m_btnLeftRightSwing1->show();
                break;
        case LEFTRIGHT_LEFT_SWING:
                leftRightSwingAllShow(false);
                ui->m_btnLeftRightSwing2->show();
                break;
        case LEFTRIGHT_MID_SWING:
                leftRightSwingAllShow(false);
                ui->m_btnLeftRightSwing3->show();
                break;
        case LEFTRIGHT_RIGHT_SWING:
                leftRightSwingAllShow(false);
                ui->m_btnLeftRightSwing4->show();
                break;
        case LEFTRIGHT_RIGHT_RIGHT_SWING:
                leftRightSwingAllShow(false);
                ui->m_btnLeftRightSwing5->show();
                break;
        case LEFTRIGHT_RIGHT_TWO_SWING:
                leftRightSwingAllShow(false);
                ui->m_btnLeftRightSwing1->show();
                ui->m_btnLeftRightSwing5->show();
                break;
        case LEFTRIGHT_SWING:
                ui->m_btnLeftRightSwing1->hide();
                ui->m_btnLeftRightSwing2->show();
                ui->m_btnLeftRightSwing3->hide();
                ui->m_btnLeftRightSwing4->show();
                ui->m_btnLeftRightSwing5->hide();
           break;
    default:break;
    }
}

void HouseCtlConditon::leftRightSwingAllShow(bool isShow)
{
    if(isShow)
    {
        ui->m_btnLeftRightSwing1->show();
        ui->m_btnLeftRightSwing2->show();
        ui->m_btnLeftRightSwing3->show();
        ui->m_btnLeftRightSwing4->show();
        ui->m_btnLeftRightSwing5->show();
    }
    else
    {
        ui->m_btnLeftRightSwing1->hide();
        ui->m_btnLeftRightSwing2->hide();
        ui->m_btnLeftRightSwing3->hide();
        ui->m_btnLeftRightSwing4->hide();
        ui->m_btnLeftRightSwing5->hide();
    }
}

void HouseCtlConditon::upDownSwingAllShow(bool isShow)
{
    if(isShow)
    {
        ui->m_btnUpDownSwing1->show();
        ui->m_btnUpDownSwing2->show();
        ui->m_btnUpDownSwing3->show();
        ui->m_btnUpDownSwing4->show();
        ui->m_btnUpDownSwing5->show();
    }
    else
    {
        ui->m_btnUpDownSwing1->hide();
        ui->m_btnUpDownSwing2->hide();
        ui->m_btnUpDownSwing3->hide();
        ui->m_btnUpDownSwing4->hide();
        ui->m_btnUpDownSwing5->hide();
    }
}

void HouseCtlConditon::windSpeedSelect(int windSpeed)
{
    switch (windSpeed)
    {
        case AUTO_LEVEL:ui->m_lbWindSpeed->setText("自动");break;
        case FIRST_LEVEL:ui->m_lbWindSpeed->setText("1");break;
        case SECOND_LEVEL:ui->m_lbWindSpeed->setText("2");break;
        case THIRD_LEVEL:ui->m_lbWindSpeed->setText("3");break;
    default:break;
    }
}

void HouseCtlConditon::tempShowModeSelect(int tempShow)
{
    switch(tempShow)
    {
        case NONE:
            ui->m_btnTempSetIcon->hide();
            ui->m_btnTempIndoorIcon->hide();
            ui->m_btnTempOutdoorIcon->hide();
            break;
        case SET:
            ui->m_btnTempSetIcon->show();
            ui->m_btnTempIndoorIcon->hide();
            ui->m_btnTempOutdoorIcon->hide();
            break;
        case INDOOR:
            ui->m_btnTempSetIcon->hide();
            ui->m_btnTempIndoorIcon->show();
            ui->m_btnTempOutdoorIcon->hide();
            break;
        case OUTDOOR:
            ui->m_btnTempSetIcon->show();
            ui->m_btnTempIndoorIcon->hide();
            ui->m_btnTempOutdoorIcon->show();
            break;
        default:break;
    }
}

void HouseCtlConditon::on_m_btnSwitch_clicked()
{
    qDebug()<<"btnSwitch";
    //关睡眠
    if(m_currentSleep == 1)
    {
        operateCode1[7] = '0';
        m_currentSleep = 0;
        updateModeCode();
    }

    //开辅热
    if(m_currentRunMode == HOT)
    {
        if(operateCode1[23] == '1')
        {
            operateCode1[23] = '0';
            updateModeCode();
        }
    }

    //关定时
    if(m_timerBoot->isActive())
    {
        m_timerBoot->stop();
    }
    if(m_timerRun->isActive())
    {
        m_timerRun->stop();
        QString szTimerCode = "00000000";
        stringReplace(operateCode1,szTimerCode,12);
        stringReplace(audoMode1,szTimerCode,12);
        stringReplace(coldMode1,szTimerCode,12);
        stringReplace(wetMode1,szTimerCode,12);
        stringReplace(windMode1,szTimerCode,12);
        stringReplace(hotMode1,szTimerCode,12);

        generateCheckNum(checkNumArray);
        stringReplace(operateCode2,checkNumArray,28);

        updateAllFrontCode();
        updateModeCode();
    }

    //修改对应码
    if(operateCode1[3] == '1')
    {
        operateCode1[3] = '0';
    }
    else
    {
        operateCode1[3] = '1';
    }

    if(operateCode2[31] == '1')
    {
        operateCode2[31] = '0';
    }
    else
    {
        operateCode2[31] = '1';
    }

    //更新界面发送命令
    updateInterface(operateCode1,operateCode2);
    sendUartMsg(operateCode1,operateCode2);
}

void HouseCtlConditon::on_m_btnMode_clicked()
{
    //关超强
    if(m_currentStrong == 1)
    {
        operateCode1[20] = '0';
        m_currentStrong = 0;
        updateModeCode();
    }

    //关睡眠
    if(m_currentSleep == 1)
    {
        operateCode1[7] = '0';
        m_currentSleep = 0;
        updateModeCode();
    }

    //开辅热
    if((m_currentRunMode == HOT)&&(operateCode1[23] == '1'))
    {
        operateCode1[23] = '0';
        updateModeCode();
    }

    //关制冷模式节能
    if((m_currentRunMode == COLD)&&(operateCode2[26] == '1'))
    {
        operateCode2[26] = '0';
        updateModeCode();
    }

    if(m_currentRunMode < 4)
    {
        m_currentRunMode = m_currentRunMode + 1;
    }
    else
    {
        m_currentRunMode = 0;
    }

    switch (m_currentRunMode)
    {
    case AUTO:
        operateCode1 = audoMode1;
        operateCode2 = audoMode2;
        break;
    case COLD:
        operateCode1 = coldMode1;
        operateCode2 = coldMode2;
        break;
    case WET:
        operateCode1 = wetMode1;
        operateCode2 = wetMode2;
        break;
    case WIND:
        operateCode1 = windMode1;
        operateCode2 = windMode2;
        break;
    case HOT:
        operateCode1 = hotMode1;
        operateCode2 = hotMode2;
        break;
    default:break;
    }

    updateInterface(operateCode1,operateCode2);
    generateCheckNum(checkNumArray);
    stringReplace(operateCode2,checkNumArray,28);

    updateModeCode();
    sendUartMsg(operateCode1,operateCode2);
}

void HouseCtlConditon::on_m_btnAdd_clicked()
{
    if(m_timerBoot->isActive())
    {
        //定时时调整定时时间
        if(m_currentTimers <24)
        {
            m_currentTimers = m_currentTimers + 0.5;
            m_timerBootTimes = 0;
            ui->m_lbTimer->setText(QString::number(m_currentTimers));
        }
        else
        {
            m_currentTimers = 0.5;
        }
    }
    else
    {
        //调整温度
        if(m_currentRunMode != AUTO)
        {
            if(m_currentTmp < 30)
            {
                m_currentTmp = m_currentTmp + 1;
                stringReplace(operateCode1,temperature[m_currentTmp - 16],8);

                generateCheckNum(checkNumArray);
                stringReplace(operateCode2,checkNumArray,28);

                updateInterface(operateCode1,operateCode2);
                updateModeCode();
                sendUartMsg(operateCode1,operateCode2);
            }
            else
            {
                return ;
            }
        }
        else
        {
        }
    }

}

void HouseCtlConditon::on_m_btnSub_clicked()
{
    if(m_timerBoot->isActive())
    {
        if(m_currentTimers >0.5)
        {
            m_currentTimers = m_currentTimers - 0.5;
            m_timerBootTimes = 0;
            ui->m_lbTimer->setText(QString::number(m_currentTimers));
        }
        else
        {
            m_currentTimers = 24;
        }

    }
    else
    {
        if(m_currentRunMode != AUTO)
        {
            if(m_currentTmp > 16)
            {
                m_currentTmp = m_currentTmp - 1;
                stringReplace(operateCode1,temperature[m_currentTmp - 16],8);

                generateCheckNum(checkNumArray);
                stringReplace(operateCode2,checkNumArray,28);

                updateInterface(operateCode1,operateCode2);
                updateModeCode();
                sendUartMsg(operateCode1,operateCode2);
            }
            else
            {
                return ;
            }
        }
    }
}

void HouseCtlConditon::on_m_btnWindSpeed_clicked()
{
    if(m_currentWindSpeed < 3)
    {
        m_currentWindSpeed = m_currentWindSpeed + 1;
    }
    else
    {
        m_currentWindSpeed = 0;
    }
    QString windSpeedCode = windSpeed[m_currentWindSpeed];
    stringReplace(operateCode1,windSpeedCode,4);

    updateInterface(operateCode1,operateCode2);
    updateModeCode();
    sendUartMsg(operateCode1,operateCode2);
}

void HouseCtlConditon::on_m_btnLight_clicked()
{
    if(m_currentLight == 1)
    {
        m_currentLight = 0;

        operateCode1[21] = '0';
        audoMode1[21] = '0';
        coldMode1[21] = '0';
        wetMode1[21] = '0';
        windMode1[21] = '0';
        hotMode1[21] = '0';
    }
    else
    {
        m_currentLight = 1;

        operateCode1[21] = '1';
        audoMode1[21] = '1';
        coldMode1[21] = '1';
        wetMode1[21] = '1';
        windMode1[21] = '1';
        hotMode1[21] = '1';
    }

    updateInterface(operateCode1,operateCode2);
    updateAllFrontCode();
    sendUartMsg(operateCode1,operateCode2);
}

void HouseCtlConditon::on_m_btnStrong_clicked()
{
    if((m_currentRunMode == COLD)||(m_currentRunMode == HOT))
    {
        if(m_currentStrong == 1)
        {
            m_currentStrong = 0;
            operateCode1[20] = '0';
        }
        else
        {
            m_currentStrong = 1;
            operateCode1[20] = '1';
        }

        updateInterface(operateCode1,operateCode2);
        updateModeCode();
        sendUartMsg(operateCode1,operateCode2);
    }
}

void HouseCtlConditon::on_m_btnSleep_clicked()
{
    if((m_currentRunMode == COLD)||(m_currentRunMode == HOT)||(m_currentRunMode == WET))
    {
        if(m_currentSleep == 1)
        {
            m_currentSleep = 0;
            operateCode1[7] = '0';
        }
        else
        {
            m_currentSleep = 1;
            operateCode1[7] = '1';
        }

        updateInterface(operateCode1,operateCode2);
        updateModeCode();
        sendUartMsg(operateCode1,operateCode2);
    }
}

void HouseCtlConditon::on_m_btnTempShow_clicked()
{
    if(m_currentTempShowMode <3)
    {
        m_currentTempShowMode = m_currentTempShowMode + 1;
    }
    else
    {
        m_currentTempShowMode = 0;
    }

    stringReplace(operateCode2,tempShowMode[m_currentTempShowMode],8);
    stringReplace(audoMode2,tempShowMode[m_currentTempShowMode],8);
    stringReplace(coldMode2,tempShowMode[m_currentTempShowMode],8);
    stringReplace(wetMode2,tempShowMode[m_currentTempShowMode],8);
    stringReplace(windMode2,tempShowMode[m_currentTempShowMode],8);
    stringReplace(hotMode2,tempShowMode[m_currentTempShowMode],8);

    updateInterface(operateCode1,operateCode2);
    updateAllRearCode();
    sendUartMsg(operateCode1,operateCode2);
}

void HouseCtlConditon::on_m_btnUpDownSwing_clicked()
{
    //扫风状态
    if(m_currentUpDownSwingMode < 9)
    {
        m_currentUpDownSwingMode = m_currentUpDownSwingMode + 1;
    }
    else
    {
        m_currentUpDownSwingMode = 0;
    }

    stringReplace(operateCode2,upDownSwing[m_currentUpDownSwingMode],0);

    //扫风标志位
    if((m_currentUpDownSwingMode == 1)||((m_currentUpDownSwingMode >= 7)&&(m_currentUpDownSwingMode <= 9)))
    {
        operateCode1[6] = '1';
    }
    else
    {
        operateCode1[6] = '0';
    }

    updateInterface(operateCode1,operateCode2);
    updateModeCode();
    sendUartMsg(operateCode1,operateCode2);
}

void HouseCtlConditon::on_m_btnLeftRightSwing_clicked()
{
    //扫风状态
    if(m_currentLeftRightSwingMode < 8)
    {
        m_currentLeftRightSwingMode = m_currentLeftRightSwingMode + 1;
    }
    else
    {
        m_currentLeftRightSwingMode = 0;
    }

    stringReplace(operateCode2,leftRightSwing[m_currentLeftRightSwingMode],4);

    //扫风标志位
    if((m_currentLeftRightSwingMode == 1)||(m_currentLeftRightSwingMode == 8))
    {
        operateCode1[6] = '1';
    }
    else
    {
        operateCode1[6] = '0';
    }

    generateCheckNum(checkNumArray);
    stringReplace(operateCode2,checkNumArray,28);

    updateInterface(operateCode1,operateCode2);
    updateModeCode();
    sendUartMsg(operateCode1,operateCode2);

}

void HouseCtlConditon::on_m_btnDryAuxHeat_clicked()
{
    if((m_currentRunMode == COLD)||(m_currentRunMode == WET))
    {
        //COLD，WET模式下是干燥
        if(operateCode1[23] == '1')
        {
            operateCode1[23] = '0';
            coldMode1[23] = '0';
            wetMode1[23] = '0';
        }
        else
        {
            operateCode1[23] = '1';
            coldMode1[23] = '1';
            wetMode1[23] = '1';
        }

        updateInterface(operateCode1,operateCode2);
        updateAllFrontCode();
        sendUartMsg(operateCode1,operateCode2);
    }
    else if(m_currentRunMode == HOT)
    {
        //制热模式下关辅热
        if(operateCode1[23] == '1')
        {
            operateCode1[23] = '0';
        }
        else
        {
            operateCode1[23] = '1';
        }
        updateInterface(operateCode1,operateCode2);
        updateModeCode();
        sendUartMsg(operateCode1,operateCode2);
    }
}

void HouseCtlConditon::on_m_btnSaveEnergy_clicked()
{
    if(m_currentRunMode == COLD)
    {
        if(operateCode2[26] == '1')
        {
            operateCode2[26] = '0';
        }
        else
        {
            operateCode2[26] = '1';
        }

        updateInterface(operateCode1,operateCode2);
        updateModeCode();
        sendUartMsg(operateCode1,operateCode2);
    }
}

void HouseCtlConditon::on_m_btnHealthSwap_clicked()
{
    //四种状态
    if((operateCode1[22] == '0')&&(operateCode1[24] == '0'))
    {
        operateCode1[22] = '1';
        audoMode1[22] = '1';
        coldMode1[22] = '1';
        wetMode1[22] = '1';
        windMode1[22] = '1';
        hotMode1[22] = '1';

        m_currentSwapAir = 0;
    }
    else if((operateCode1[22] == '1')&&(operateCode1[24] == '0'))
    {
        operateCode1[22] = '0';operateCode1[24] = '1';
        audoMode1[22] = '0';audoMode1[24] = '1';
        coldMode1[22] = '0';coldMode1[24] = '1';
        wetMode1[22] = '0';wetMode1[24] = '1';
        windMode1[22] = '0';windMode1[24] = '1';
        hotMode1[22] = '0';hotMode1[24] = '1';

        m_currentSwapAir = 1;
    }
    else if((operateCode1[22] == '0')&&(operateCode1[24] == '1'))
    {
        operateCode1[22] = '1';
        audoMode1[22] = '1';
        coldMode1[22] = '1';
        wetMode1[22] = '1';
        windMode1[22] = '1';
        hotMode1[22] = '1';

        m_currentSwapAir = 1;
    }
    else if((operateCode1[22] == '1')&&(operateCode1[24] == '1'))
    {
        operateCode1[22] = '0';operateCode1[24] = '0';
        audoMode1[22] = '0';audoMode1[24] = '0';
        coldMode1[22] = '0';coldMode1[24] = '0';
        wetMode1[22] = '0';wetMode1[24] = '0';
        windMode1[22] = '0';windMode1[24] = '0';
        hotMode1[22] = '0';hotMode1[24] = '0';

        m_currentSwapAir = 0;
    }

    generateCheckNum(checkNumArray);
    stringReplace(operateCode2,checkNumArray,28);

    updateInterface(operateCode1,operateCode2);
    updateAllFrontCode();
    updateModeCode();

    sendUartMsg(operateCode1,operateCode2);
}

void HouseCtlConditon::on_m_btnTimer_clicked()
{
    /*
    if(m_timerRun->isActive())
    {
        m_timerRun->stop();
        QString szTimerCode = "00000000";

        stringReplace(operateCode1,szTimerCode,12);
        stringReplace(audoMode1,szTimerCode,12);
        stringReplace(coldMode1,szTimerCode,12);
        stringReplace(wetMode1,szTimerCode,12);
        stringReplace(windMode1,szTimerCode,12);
        stringReplace(hotMode1,szTimerCode,12);

        generateCheckNum(checkNumArray);
        stringReplace(operateCode2,checkNumArray,28);

        updateInterface(operateCode1,operateCode2);
        updateAllFrontCode();
        updateModeCode();
        sendUartMsg(operateCode1,operateCode2);
    }
    else
    {
        if(m_timerBoot->isActive())
        {
            m_timerBoot->stop();
            QString szTimerCode = "00000000";
            qDebug()<<"m_currentTimers"<<m_currentTimers;
            int shiwei = int(m_currentTimers) / 10 ;
            int gewei = int(m_currentTimers) % 10 ;
            for(int i=0;i<3;i++)
            {
                if(shiwei == i)
                {
                    stringReplace(szTimerCode,timerData[i],1);
                }
            }
            for(int i=0;i<10;i++)
            {
                if(gewei == i)
                {
                    stringReplace(szTimerCode,temperature[i],4);
                }
            }
            if((m_currentTimers - gewei - 10*shiwei)>0)
            {
                szTimerCode[0] = '1';
            }
            qDebug()<<"szTimerCode"<<szTimerCode;

            stringReplace(operateCode1,szTimerCode,12);
            stringReplace(audoMode1,szTimerCode,12);
            stringReplace(coldMode1,szTimerCode,12);
            stringReplace(wetMode1,szTimerCode,12);
            stringReplace(windMode1,szTimerCode,12);
            stringReplace(hotMode1,szTimerCode,12);

            generateCheckNum(checkNumArray);
            stringReplace(operateCode2,checkNumArray,28);

            updateInterface(operateCode1,operateCode2);
            updateAllFrontCode();
            updateModeCode();
            sendUartMsg(operateCode1,operateCode2);
            qDebug()<<operateCode1;
            qDebug()<<operateCode2;

            m_timerRunTimes = 0;
            m_timerRun->start(60000);
        }
        else
        {
            m_timerBootTimes = 0;
            m_timerBoot->start(500);
        }
    }*/
}

void HouseCtlConditon::isStartRunTimer()
{
    m_timerRunTimes++;

    if(m_timerRunTimes > m_currentTimers * 60)
    {
        on_m_btnSwitch_clicked();
    }
}

void HouseCtlConditon::isStartBootTimer()
{
    qDebug()<<"isStartTimeing";
    m_timerBootTimes++;
    ui->m_lbTimer->show();

    if(ui->m_lbTimer2->isHidden())
    {
        //qDebug()<<"isHidden";
        ui->m_lbTimer2->setVisible(true);
    }
    else
    {
       // qDebug()<<"isShow";
        ui->m_lbTimer2->setVisible(false);
    }

    if(m_timerBootTimes >= 10)
    {
        m_timerBootTimes = 0;
        ui->m_lbTimer->hide();
        ui->m_lbTimer2->hide();
        m_timerBoot->stop();
    }
}


void HouseCtlConditon::stringReplace(QString &origin,QString object,int n)
{
    int i;
    for (i = 0;i<object.length(); i++)
    {
          origin[i + n] = object[i];
    }
}

void HouseCtlConditon::generateCheckNum(QString &checkNumArray)
{
    QString timerCheckNum = operateCode1.mid(16,4);
    int timerCheck = 0;
    for(int i = 0;i<10;i++)
    {
        if(timerCheckNum == temperature[i])
        {
            timerCheck = i;
        }
    }
    int checkNum;
    checkNum = m_currentRunMode + m_currentTmp + 4 + leftRightSwingValue[m_currentLeftRightSwingMode]
            + m_currentSwapAir + timerCheck;

    for (int i = 0; i < 4; i++)
    {
        if((checkNum >> i) & 0x01)
        {
            checkNumArray[i] = '1';
        }
        else
        {
            checkNumArray[i] = '0';
        }
    }
}

void HouseCtlConditon::updateModeCode()
{
    QSqlQuery query;
    switch (m_currentRunMode)
    {
        case AUTO:  audoMode1 = operateCode1;
                    audoMode2 = operateCode2;
                    query.exec("update aircondition set Auto1 = '"+audoMode1+"' where MacAddr = '"+m_szAddr+"'");
                    query.exec("update aircondition set Auto2 = '"+audoMode2+"' where MacAddr = '"+m_szAddr+"'");
                    break;
        case COLD:  coldMode1 = operateCode1;
                    coldMode2 = operateCode2;
                    query.exec("update aircondition set Cold1 = '"+coldMode1+"' where MacAddr = '"+m_szAddr+"'");
                    query.exec("update aircondition set Cold2 = '"+coldMode2+"' where MacAddr = '"+m_szAddr+"'");
                    break;
        case WET:   wetMode1 = operateCode1;
                    wetMode2 = operateCode2;
                    query.exec("update aircondition set Wet1 = '"+wetMode1+"' where MacAddr = '"+m_szAddr+"'");
                    query.exec("update aircondition set Wet2 = '"+wetMode2+"' where MacAddr = '"+m_szAddr+"'");
                    break;
        case WIND:  windMode1 = operateCode1;
                    windMode2 = operateCode2;
                    query.exec("update aircondition set Wind1 = '"+windMode1+"' where MacAddr = '"+m_szAddr+"'");
                    query.exec("update aircondition set Wind2 = '"+windMode2+"' where MacAddr = '"+m_szAddr+"'");
                    break;
        case HOT:   hotMode1 = operateCode1;
                    hotMode2 = operateCode2;
                    query.exec("update aircondition set Hot1 = '"+hotMode1+"' where MacAddr = '"+m_szAddr+"'");
                    query.exec("update aircondition set Hot2 = '"+hotMode2+"' where MacAddr = '"+m_szAddr+"'");
                    break;
    default:break;
    }
    query.exec("update aircondition set CurrentCode1 = '"+operateCode1+"' where MacAddr = '"+m_szAddr+"'");
    query.exec("update aircondition set CurrentCode2 = '"+operateCode2+"' where MacAddr = '"+m_szAddr+"'");
}

void HouseCtlConditon::updateAllFrontCode()
{
    QSqlQuery query;

    query.exec("update aircondition set CurrentCode1 = '"+operateCode1+"' where MacAddr = '"+m_szAddr+"'");
    query.exec("update aircondition set audoMode1 = '"+operateCode1+"' where MacAddr = '"+m_szAddr+"'");
    query.exec("update aircondition set coldMode1 = '"+operateCode1+"' where MacAddr = '"+m_szAddr+"'");
    query.exec("update aircondition set wetMode1 = '"+operateCode1+"' where MacAddr = '"+m_szAddr+"'");
    query.exec("update aircondition set windMode1 = '"+operateCode1+"' where MacAddr = '"+m_szAddr+"'");
    query.exec("update aircondition set hotMode1 = '"+operateCode1+"' where MacAddr = '"+m_szAddr+"'");
}

void HouseCtlConditon::updateAllRearCode()
{
    QSqlQuery query;

    query.exec("update aircondition set CurrentCode2 = '"+operateCode2+"' where MacAddr = '"+m_szAddr+"'");
    query.exec("update aircondition set audoMode2 = '"+operateCode2+"' where MacAddr = '"+m_szAddr+"'");
    query.exec("update aircondition set coldMode2 = '"+operateCode2+"' where MacAddr = '"+m_szAddr+"'");
    query.exec("update aircondition set wetMode2 = '"+operateCode2+"' where MacAddr = '"+m_szAddr+"'");
    query.exec("update aircondition set windMode2 = '"+operateCode2+"' where MacAddr = '"+m_szAddr+"'");
    query.exec("update aircondition set hotMode2 = '"+operateCode2+"' where MacAddr = '"+m_szAddr+"'");
}

void HouseCtlConditon::sendUartMsg(QString commandFront,QString commandRear)
{
    //将空调前35位转换为字节
    QString dataFront[5];
    for(int j=0;j<5;j++)
    {
        dataFront[j] = commandFront.mid(0+j*8,8);
    }
    uint8_t commandData1[5] = {0};
    for(int j=0;j<5;j++)
    {
        for(int i = 0;i < 8;i++)
        {
            commandData1[j] = commandData1[j]<<1;
            if(dataFront[j][i] == '1')
            {
                commandData1[j] = commandData1[j] | 0x01;
            }
        }
       // qDebug()<<commandData1[j];
    }

    //将空调后32位转换为字节
    QString dataRear[4];
    for(int j=0;j<4;j++)
    {
        dataRear[j] = commandRear.mid(0+j*8,8);
    }
    uint8_t commandData2[4];
    for(int j=0;j<4;j++)
    {
        for(int i = 0;i < 8;i++)
        {
            commandData2[j] = commandData2[j]<<1;
            if(dataRear[j][i] == '1')
            {
                commandData2[j] = commandData2[j] | 0x01;
            }
        }
        //qDebug()<<commandData2[j];
    }

    //将字符串短地址转换为整型，得到串口消息类型数据
    uartSendMsg msg;
    for(int i = 0;i < 4;i++)
    {
       msg.shortAddr =  msg.shortAddr<<4;
       if(m_szShortAddr[i].toLatin1() - 48 <10)
       {
           msg.shortAddr = msg.shortAddr | (m_szShortAddr[i].toLatin1() - 48);
       }
       else
       {
           msg.shortAddr = msg.shortAddr | (m_szShortAddr[i].toLatin1() - 87);
       }
    }
    //qDebug()<<msg.shortAddr;

    uint8_t byteShortAddr[2];
    byteShortAddr[0] = msg.shortAddr >>8;
    byteShortAddr[1] = msg.shortAddr;

    msg.body.msgType = SMARTHOME_AIRCONDITIONMSG;
    msg.body.len = 9;

    //串口消息转换为比特流
    QByteArray uartData;
    uartData.append(msg.header);
    uartData.append(byteShortAddr[0]);
    uartData.append(byteShortAddr[1]);
    uartData.append( msg.body.msgType);
    uartData.append( msg.body.len);
    for(int i = 0;i<5;i++)
    {
        uartData.append(commandData1[i]);
    }
    for(int i = 0;i<4;i++)
    {
        uartData.append(commandData2[i]);
    }

    uartData.append(msg.tailer);

    pQSerial->write(uartData);
}
