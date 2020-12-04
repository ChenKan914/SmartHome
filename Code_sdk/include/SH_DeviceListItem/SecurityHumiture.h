#ifndef SECURITYHUMITURE_H
#define SECURITYHUMITURE_H

#include <QWidget>

namespace Ui {
class SecurityHumiture;
}

class SecurityHumiture : public QWidget
{
    Q_OBJECT

public:
    explicit SecurityHumiture(QWidget *parent = nullptr);
    ~SecurityHumiture();

    void init();

private:
    Ui::SecurityHumiture *ui;
};

#endif // SECURITYHUMITURE_H
