#include "loginpage.h"
#include <QDebug>
#include <QMessageBox>
#include <QTabBar>
#include "ui_loginpage.h"

LoginPage::LoginPage(QWidget *parent) : QWidget(parent), ui(new Ui::LoginPage) {
    ui->setupUi(this);

    setMinimumSize(0, 0);
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setAlignment(Qt::AlignCenter);

    QWidget *centerWidget = new QWidget(this);
    QVBoxLayout *centerLayout = new QVBoxLayout(centerWidget);
    centerLayout->setAlignment(Qt::AlignCenter);
    centerLayout->setSpacing(10);

    QHBoxLayout *switchLayout = new QHBoxLayout;
    switchLayout->setAlignment(Qt::AlignCenter);
    switchLayout->setSpacing(20);
    QString btnStyle =
        "QPushButton {"
        "    min-width: 80px;"
        "    max-width: 80px;"
        "    min-height: 30px;"
        "    max-height: 30px;"
        "}";

    QString btnStyleChosen =
        "QPushButton {"
        "    min-width: 80px;"
        "    max-width: 80px;"
        "    min-height: 30px;"
        "    max-height: 30px;"
        "    background-color: rgb(254,224,224);"
        "}";
    ui->loginSwitchButton->setStyleSheet(btnStyleChosen);
    ui->registrationSwitchButton->setStyleSheet(btnStyle);

    switchLayout->addWidget(ui->loginSwitchButton);
    switchLayout->addWidget(ui->registrationSwitchButton);

    ui->stackedWidget->setMinimumSize(300, 250);
    ui->stackedWidget->setMaximumSize(300, 250);

    centerLayout->addStretch();
    centerLayout->addLayout(switchLayout);
    centerLayout->addSpacing(20);
    centerLayout->addWidget(ui->stackedWidget);
    centerLayout->addStretch();

    mainLayout->addWidget(centerWidget);

    ui->loginPage->setMinimumSize(300, 250);
    ui->loginPage->setMaximumSize(300, 250);

    QVBoxLayout *loginLayout =
        qobject_cast<QVBoxLayout *>(ui->loginPage->layout());
    if (loginLayout) {
        loginLayout->setAlignment(Qt::AlignCenter);
        loginLayout->setSpacing(8);
    }

    ui->registrationPage->setMinimumSize(300, 250);
    ui->registrationPage->setMaximumSize(300, 250);

    QVBoxLayout *regLayout =
        qobject_cast<QVBoxLayout *>(ui->registrationPage->layout());
    if (regLayout) {
        regLayout->setAlignment(Qt::AlignCenter);
        regLayout->setSpacing(8);
    }

    ui->stackedWidget->setCurrentWidget(ui->loginPage);

    connect(ui->loginSwitchButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->loginPage);
        ui->loginSwitchButton->setStyleSheet(btnStyleChosen);
        ui->registrationSwitchButton->setStyleSheet(btnStyle);
    });
    connect(ui->registrationSwitchButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->registrationPage);
        ui->registrationSwitchButton->setStyleSheet(btnStyleChosen);
        ui->loginSwitchButton->setStyleSheet(btnStyle);
    });

    connect(
        ui->clearButtonLogin, &QPushButton::clicked, this,
        &LoginPage::clearClicked
    );
    connect(
        ui->clearButtonRegistration, &QPushButton::clicked, this,
        &LoginPage::clearClicked
    );
    connect(
        ui->loginButton, &QPushButton::clicked, this, &LoginPage::okClicked
    );

    connect(
        ui->registrationButton, &QPushButton::clicked, this,
        &LoginPage::registrationClicked
    );

    ui->loginInputLogin->setStyleSheet("background-color: white;");
    ui->passwordInputLogin->setStyleSheet("background-color: white;");

    ui->verticalWidgetLogin->setStyleSheet(
        "background-color: rgb(248, 199, 199);"
        "border-radius: 5px;"
    );

    ui->loginInputRegistration->setStyleSheet("background-color: white;");
    ui->passwordInputRegistration->setStyleSheet("background-color: white;");

    ui->verticalWidgetRegistration->setStyleSheet(
        "background-color: rgb(248, 199, 199);"
        "border-radius: 5px;"
    );

    ui->studentButton->setStyleSheet(
        "QRadioButton::indicator {"
        "    width: 13px;"
        "    height: 13px;"
        "    border-radius: 7px;"
        "    border: 1px solid #999;"
        "    background-color: white;"
        "}"
        "QRadioButton::indicator:checked {"
        "    width: 13px;"
        "    height: 13px;"
        "    border-radius: 7px;"
        "    border: 1px solid white;"
        "    background-color: #999;"
        "}"
    );

    ui->teacherButton->setStyleSheet(ui->studentButton->styleSheet());
}

LoginPage::~LoginPage() {
    delete ui;
}

void LoginPage::clearClicked() {
    if (ui->stackedWidget->currentIndex() == 0) {
        ui->loginInputLogin->clear();
        ui->passwordInputLogin->clear();
    } else if (ui->stackedWidget->currentIndex() == 1) {
        ui->loginInputRegistration->clear();
        ui->passwordInputRegistration->clear();
    }
}

void LoginPage::okClicked() {
    QString login = ui->loginInputLogin->text();
    QString password = ui->passwordInputLogin->text();

    if (login == "" || password == "") {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
    NetworkManager::instance()->login(login, password);
}

void LoginPage::registrationClicked() {
    QString login = ui->loginInputRegistration->text();
    QString password = ui->passwordInputRegistration->text();
    QString role;
    if (ui->studentButton->isChecked()) {
        role = "student";
    } else if (ui->teacherButton->isChecked()) {
        role = "teacher";
    }

    if (login == "" || password == "") {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }

    NetworkManager::instance()->registration(login, password, role);
}

void LoginPage::clearFields() {
    ui->loginInputLogin->clear();
    ui->passwordInputLogin->clear();
    ui->loginInputRegistration->clear();
    ui->passwordInputRegistration->clear();
}
