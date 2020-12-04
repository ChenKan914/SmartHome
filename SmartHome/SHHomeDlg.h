#ifndef SHHOMEDLG_H
#define SHHOMEDLG_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QBuffer>

namespace Ui {
class SHHomeDlg;
}

class SHHomeDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SHHomeDlg(QWidget *parent = nullptr);
    ~SHHomeDlg();

    void init();
    void initTime();
    void initWeather();
    void setNomalStyle();
private:
    Ui::SHHomeDlg *ui;

    QTimer *m_timer;
    QNetworkAccessManager *m_pManager;  //请求句柄
    QString m_szWind;       //风力
    QString m_szTemp;        //温度
    QString m_szTempType;  //天气类型

private slots:
    void refreshTime();
    void replyFinished(QNetworkReply *reply);
};

#endif // SHHOMEDLG_H
