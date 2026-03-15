#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include "networkmanager.h"

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget {
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

private:
    Ui::LoginPage *ui;

private slots:
    void clearClicked();
    void okClicked();

    // signals:
    //     void loginSuccessful();
};

#endif  // LOGINPAGE_H
