#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loginPage = new LoginPage(this);
    taskPage = new TaskPage(this);

    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(taskPage);

    // для отладки taskPage!! потом поменять на loginPage!!
    ui->stackedWidget->setCurrentWidget(taskPage);
    // ui->stackedWidget->setCurrentWidget(loginPage);

    connect(loginPage, &LoginPage::login_successful, this, [=]() {
        ui->stackedWidget->setCurrentWidget(taskPage);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
