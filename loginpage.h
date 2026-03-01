#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

private:
    Ui::LoginPage *ui;

private slots:
    void clear_clicked();
    void ok_clicked();
};

#endif // LOGINPAGE_H
