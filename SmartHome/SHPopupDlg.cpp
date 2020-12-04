#include "SHPopupDlg.h"
#include "ui_SHPopupDlg.h"
#include <QDebug>

SHPopupDlg::SHPopupDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SHPopupDlg)
{
    ui->setupUi(this);
}

SHPopupDlg::~SHPopupDlg()
{
    qDebug()<<"~SHPopupDlg";
    delete ui;
}

void SHPopupDlg::on_buttonBox_accepted()
{
    emit btnClickSignal(true);
    this->hide();
}

void SHPopupDlg::on_buttonBox_rejected()
{
    emit btnClickSignal(false);
    this->hide();
}
