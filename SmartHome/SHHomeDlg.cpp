#include "SHHomeDlg.h"
#include "ui_SHHomeDlg.h"
#include <QFile>
#include <QBuffer>

SHHomeDlg::SHHomeDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SHHomeDlg)
{
    ui->setupUi(this);
    init();
}

SHHomeDlg::~SHHomeDlg()
{
    qDebug()<<"~SHHomeDlg";
    delete ui;
}

void SHHomeDlg::init()
{
    initTime();
    initWeather();
    setNomalStyle();
    QBuffer a;
}

void SHHomeDlg::initTime()
{
    ui->m_lbTime->setText(QTime::currentTime().toString("hh:mm"));
    ui->m_lbDate->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    ui->m_lbWeek->setText(QDate::currentDate().toString("dddd"));

    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(refreshTime()));
    m_timer->start(1000);
}

void SHHomeDlg::initWeather()
{
    m_pManager = new QNetworkAccessManager(this);  //新建QNetworkAccessManager对象
    connect(m_pManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));//关联信号和槽

    QString local_city = ui->m_lbLocation->text().trimmed(); //获得需要查询天气的城市名称
    char quest_array[256]="http://wthrcdn.etouch.cn/weather_mini?city=";
    QNetworkRequest quest;
    sprintf(quest_array,"%s%s",quest_array,local_city.toUtf8().data());
    quest.setUrl(QUrl(quest_array));
    quest.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");  /*发送get网络请求*/
    m_pManager->get(quest);
}

void SHHomeDlg::setNomalStyle()
{
    qWarning("setNomalStyle");
    QFile styleSheet(":/qss/style_SHHomeDlg.qss");
    if (!styleSheet.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");
    }
    else
    {
        this->setStyleSheet(styleSheet.readAll());
    }
}

void SHHomeDlg::replyFinished(QNetworkReply *reply)  //天气数据处理槽函数
{
    QString all = reply->readAll();

    QJsonParseError err;
    QJsonDocument json_recv = QJsonDocument::fromJson(all.toUtf8(),&err);//解析json对象
    if(!json_recv.isNull())
    {
        QJsonObject object = json_recv.object();

        if(object.contains("data"))
        {
            QJsonValue value = object.value("data");  // 获取指定 key 对应的 value
            if(value.isObject())
            {
                QJsonObject object_data = value.toObject();
                if(object_data.contains("forecast"))
                {
                    QJsonValue value = object_data.value("forecast");
                    if(value.isArray())
                    {
                        //获取显示今日天气
                        QJsonObject today_weather = value.toArray().at(0).toObject();
                        QString tmpLow = today_weather.value("low").toString();
                        QString tmpHigh = today_weather.value("high").toString();
                        QString windStrength = today_weather.value("fengli").toString();
                        windStrength.remove(0,8);
                        windStrength.remove(windStrength.length()-2,2);

                        m_szTempType = today_weather.value("type").toString();
                        m_szTemp = tmpLow.mid(tmpLow.length()-3,4) +"~"+ tmpHigh.mid(tmpHigh.length()-3,4);
                        m_szWind = today_weather.value("fengxiang").toString() + windStrength;
                        ui->m_lbWeather->setText(m_szTempType);
                        ui->m_lbTemp->setText(m_szTemp);
                        ui->m_lbWind->setText(m_szWind);

                        //获取显示明日天气
                        QJsonObject next_weather = value.toArray().at(1).toObject();
                        QString nextTmpLow = next_weather.value("low").toString();
                        QString nextTmpHigh = next_weather.value("high").toString();

                        m_szTemp = nextTmpLow.mid(nextTmpLow.length()-3,4) +"~"+ nextTmpHigh.mid(nextTmpHigh.length()-3,4);
                        m_szTempType = next_weather.value("type").toString();
                        ui->m_lbNextWeather->setText(m_szTempType); //显示天气类型
                        ui->m_lbNextTem->setText(m_szTemp);   //显示温度
                    }
                }
            }
        }
    }
    else
    {
        qDebug()<<"json_recv is NULL or is not a object !!";
    }
    reply->deleteLater(); //销毁请求对象
}

void SHHomeDlg::refreshTime()
{
    m_timer->start(1000);
    ui->m_lbTime->setText(QTime::currentTime().toString("hh:mm"));
    ui->m_lbDate->setText(QDate::currentDate().toString("yyyy年MM月dd日"));
    ui->m_lbWeek->setText(QDate::currentDate().toString("dddd"));
}
