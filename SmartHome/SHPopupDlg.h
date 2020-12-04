#ifndef SHPOPUPDLG_H
#define SHPOPUPDLG_H

#include <QDialog>

namespace Ui {
class SHPopupDlg;
}

class SHPopupDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SHPopupDlg(QWidget *parent = nullptr);
    ~SHPopupDlg();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();
private:
    Ui::SHPopupDlg *ui;
signals:
    void btnClickSignal(bool x);
};

#endif // SHPOPUPDLG_H
