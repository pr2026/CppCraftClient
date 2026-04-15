#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    showMaximized();

    loginPage = new LoginPage(this);
    taskPage = new TaskPage(this);

    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(taskPage);

    // для отладки taskPage!! потом поменять на loginPage!!
    // ui->stackedWidget->setCurrentWidget(taskPage);
    ui->stackedWidget->setCurrentWidget(loginPage);

    connect(
        NetworkManager::instance(), &NetworkManager::loginSuccess, this,
        [=]() { ui->stackedWidget->setCurrentWidget(taskPage);
        taskPage->loadTasks();
        }
    );

    connect(
        NetworkManager::instance(), &NetworkManager::registrationSuccess, this,
        [=]() { ui->stackedWidget->setCurrentWidget(taskPage);
        taskPage->loadTasks();}
    );

    connect(
        NetworkManager::instance(), &NetworkManager::error, this,
        [this](const QString &message) {
            QMessageBox::warning(this, "Ошибка", message);
        }
    );
}

MainWindow::~MainWindow() {
    delete ui;
}
