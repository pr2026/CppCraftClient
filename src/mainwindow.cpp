#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setStyleSheet("background-color: white;");
    showMaximized();

    loginPage = new LoginPage(this);
    taskPage = new TaskPage(this);
    teacherPage = new TeacherPage(this);

    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(taskPage);
    ui->stackedWidget->addWidget(teacherPage);

    // для отладки taskPage!! потом поменять на loginPage!!
    // ui->stackedWidget->setCurrentWidget(taskPage);

    // для отладки teacherPage
    ui->stackedWidget->setCurrentWidget(teacherPage);

    // ui->stackedWidget->setCurrentWidget(loginPage);

    connect(
        NetworkManager::instance(), &NetworkManager::loginSuccess, this, &MainWindow::onLoginSuccess);

    connect(
        NetworkManager::instance(), &NetworkManager::registrationSuccess, this,
        [=]() {
            // if (role )
            ui->stackedWidget->setCurrentWidget(taskPage);
        taskPage->loadTasks();
        }
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

void MainWindow::onLoginSuccess(const QString &role) {
    if (role == "student") {
        ui->stackedWidget->setCurrentWidget(taskPage);
        taskPage->loadTasks();
    } else if (role == "student") {
        ui->stackedWidget->setCurrentWidget(teacherPage);
        teacherPage->loadTasks();
    }
}
