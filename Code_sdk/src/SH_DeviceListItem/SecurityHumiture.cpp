#include "SecurityHumiture.h"
#include "ui_SecurityHumiture.h"
#include <QFile>
SecurityHumiture::SecurityHumiture(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecurityHumiture)
{
    ui->setupUi(this);
    init();
}

SecurityHumiture::~SecurityHumiture()
{
    delete ui;
}

void SecurityHumiture::init()
{
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("background-color:rgb(255,255,255,20)");
    ui->m_btnIcon->setIconSize(QSize(50,50));
    ui->m_btnIcon->setIcon(QIcon(QPixmap(":/Humiture.png")));
//    QFile styleSheet(":/style_SecurityHumiture.qss");
//    if (!styleSheet.open(QIODevice::ReadOnly))
//    {
//        qWarning("Can't open the style sheet file.");
//    }
//    else
//    {
//        this->setStyleSheet(styleSheet.readAll());
//    }

    //ui->m_btnIcon->setStyleSheet("border-image: url(:/Humiture.png)");
}
