#include "mainwindow.h"
#include <QDebug>
#include <QToolBar>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    NetworkManager::instance();

    this->setStyleSheet("background-color: white;");
    showMaximized();

    loginPage = new LoginPage(this);
    taskPage = new TaskPage(this);
    teacherPage = new TeacherPage(this);

    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(taskPage);
    ui->stackedWidget->addWidget(teacherPage);

    ui->stackedWidget->setCurrentWidget(loginPage);

    QToolBar *toolBar = new QToolBar(this);
    addToolBar(toolBar);

    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);

    logoutAction = new QAction("Exit", this);
    toolBar->addAction(logoutAction);

    QFont font;
    font.setFamily("Century Gothic");
    font.setPointSize(11);

    logoutAction->setFont(font);

    connect(
        NetworkManager::instance(), &NetworkManager::loginSuccess, this,
        &MainWindow::onLoginSuccess
    );

    connect(
        NetworkManager::instance(), &NetworkManager::registrationSuccess, this,
        &MainWindow::onRegistrationSuccess
    );

    connect(
        NetworkManager::instance(), &NetworkManager::error, this,
        [this](const QString &message) {
            QMessageBox::warning(this, "Ошибка", message);
        }
    );
    connect(
        logoutAction, &QAction::triggered, this, &MainWindow::onLogoutClicked
    );
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onLoginSuccess(
    const QString &userName,
    const QString &role,
    int userId
) {
    NetworkManager::instance()->setCurrentUser(userName, role, userId);
    logoutAction->setVisible(true);

    if (role == "student") {
        ui->stackedWidget->setCurrentWidget(taskPage);
        taskPage->loadTasks();
    } else if (role == "teacher") {
        ui->stackedWidget->setCurrentWidget(teacherPage);
        teacherPage->loadTasks();
    }
}

void MainWindow::onRegistrationSuccess(
    const QString &userName,
    const QString &role,
    int userId
) {
    NetworkManager::instance()->setCurrentUser(userName, role, userId);
    logoutAction->setVisible(true);

    if (role == "student") {
        ui->stackedWidget->setCurrentWidget(taskPage);
        taskPage->loadTasks();
    } else if (role == "teacher") {
        ui->stackedWidget->setCurrentWidget(teacherPage);
        teacherPage->loadTasks();
    }
}

void MainWindow::onLogoutClicked() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, "Logout", "Are you sure you want to leave?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        NetworkManager::instance()->logout();
        logoutAction->setVisible(false);
        ui->stackedWidget->setCurrentWidget(loginPage);
        loginPage->clearFields();
    }
}
