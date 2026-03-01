#include "loginpage.h"
#include "ui_loginpage.h"
#include <QDebug>
#include <QMessageBox>

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    connect(ui->clear_button, &QPushButton::clicked, this, &LoginPage::clear_clicked);
    connect(ui->ok_button, &QPushButton::clicked, this, &LoginPage::ok_clicked);

    ui->login_line->setStyleSheet("background-color: white;");
    ui->password_line->setStyleSheet("background-color: white;");

    ui->verticalWidget->setStyleSheet(
        "background-color: rgb(248, 199, 199);"
        "border-radius: 5px;"
        );
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::clear_clicked()
{
    ui->login_line->clear();
    ui->password_line->clear();
}

void LoginPage::ok_clicked()
{
    QString login = ui->login_line->text();
    QString password = ui->password_line->text();

    if (login == "" || password == "") {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    } else {
        QMessageBox::information(this, "Ура", "Вы зарегистрировались!");
        hide();
    }
}
