#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // qDebug() << "До создания NetworkManager";
    // NetworkManager* nm = NetworkManager::instance();
    // qDebug() << "После создания NetworkManager, адрес:" << nm;

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
        [=]() { ui->stackedWidget->setCurrentWidget(taskPage); }
    );

    connect(
        NetworkManager::instance(), &NetworkManager::loginError, this,
        [this](const QString &message) {
            QMessageBox::warning(this, "Ошибка", message);
        }
    );
}

MainWindow::~MainWindow() {
    delete ui;
}
