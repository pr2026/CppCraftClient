#include "loginpage.h"
#include <QDebug>
#include <QMessageBox>
#include "ui_loginpage.h"

LoginPage::LoginPage(QWidget *parent) : QWidget(parent), ui(new Ui::LoginPage) {
    ui->setupUi(this);
    connect(
        ui->clearButton, &QPushButton::clicked, this, &LoginPage::clearClicked
    );
    connect(ui->okButton, &QPushButton::clicked, this, &LoginPage::okClicked);

    ui->loginInput->setStyleSheet("background-color: white;");
    ui->passwordInput->setStyleSheet("background-color: white;");

    ui->verticalWidget->setStyleSheet(
        "background-color: rgb(248, 199, 199);"
        "border-radius: 5px;"
    );
}

LoginPage::~LoginPage() {
    delete ui;
}

void LoginPage::clearClicked() {
    ui->loginInput->clear();
    ui->passwordInput->clear();
}

void LoginPage::okClicked() {
    // emit NetworkManager::instance()->loginSuccess();  // for debugging!!

    QString login = ui->loginInput->text();
    QString password = ui->passwordInput->text();

    if (login == "" || password == "") {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
    NetworkManager::instance()->login(login, password);
}
