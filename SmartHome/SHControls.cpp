#include <SHControls.h>

SHToolButton::SHToolButton(QWidget *parent):
    QToolButton (parent)
{

}

void SHToolButton::setIconDeviceMgrDlg(QString Type)
{
    if(Type == "电灯")
    {
        this->setIcon(QIcon(QPixmap(":/images/DeviceMgrDlg/Lamp.png")));
    }
    else if(Type == "窗帘")
    {
        this->setIcon(QIcon(QPixmap(":/images/DeviceMgrDlg/Curtain.png")));
    }
    else if(Type == "空调")
    {
        this->setIcon(QIcon(QPixmap(":/images/DeviceMgrDlg/Condition.png")));
    }
    else if(Type == "电视")
    {
        this->setIcon(QIcon(QPixmap(":/images/DeviceMgrDlg/Television.png")));
    }
    else if(Type == "温湿度计")
    {
        this->setIcon(QIcon(QPixmap(":/images/DeviceMgrDlg/Hygrothermograph.png")));
    }

}

SHListWidgetItem::SHListWidgetItem(QListWidget *view,int Tpye):
   QListWidgetItem(view,Tpye)
{

}

SHListWidgetItem::~SHListWidgetItem()
{

}

SHStringList *SHStringList::removeRepeatItem()
{
    for (int i = 0; i < this->count(); i++)
    {
        for (int k = i + 1; k <  this->count(); k++)
        {
            if ( this->at(i) ==  this->at(k))
            {
                this->removeAt(k);
                k--;
            }
        }
    }
    return this;
}
