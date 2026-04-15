#include "taskpage.h"
#include "ui_taskpage.h"

TaskPage::TaskPage(QWidget *parent) : QWidget(parent), ui(new Ui::TaskPage) {
    ui->setupUi(this);
    this->setContentsMargins(0, 0, 0, 0);
    ui->rightWidget->setContentsMargins(0, 0, 0, 0);

    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 1);
    ui->splitter->setSizes({400, width() - 400});

    ui->splitter_2->setStretchFactor(0, 1);
    ui->splitter_2->setStretchFactor(1, 2);
    ui->splitter_2->setSizes({100, height() - 130});

    ui->taskCondition->setWordWrapMode(QTextOption::WordWrap);

    ui->tasksList->setUniformItemSizes(true);
    ui->tasksList->setGridSize(QSize(200, 35));

    QFont font;
    font.setPointSize(11);
    ui->tasksList->setFont(font);
    ui->taskCondition->setFont(font);

    ui->tasksList->setStyleSheet(
        "QListWidget::item:selected {"
        "    background-color: #A2D2FF;"
        "    color: black;"
        "}"
        "QListWidget::item:hover:!selected {"
        "    background-color: #D1E8FC;"
        "}"
        "QListWidget:focus {"
        "    outline: none;"
        "}"
    );

    connect(
        NetworkManager::instance(), &NetworkManager::tasksLoadSuccess, this,
        &TaskPage::tasksLoaded
    );
    connect(
        NetworkManager::instance(), &NetworkManager::taskDetailsLoadSuccess, this,
        &TaskPage::taskDetailsLoaded
    );
    connect(
        ui->tasksList, &QListWidget::itemClicked, this,
        &TaskPage::taskSelected
    );
    connect(
        ui->clearButton, &QPushButton::clicked, this, &TaskPage::clearClicked
    );
    connect(
        ui->submitButton, &QPushButton::clicked, this, &TaskPage::submitClicked
    );
    connect(NetworkManager::instance(), &NetworkManager::solutionResult, this, &TaskPage::solutionResult);
    connect(NetworkManager::instance(), &NetworkManager::solutionError, this, &TaskPage::solutionError);
}

TaskPage::~TaskPage() {
    delete ui;
}

void TaskPage::loadTasks() {
    NetworkManager::instance()->loadTasks();
}

void TaskPage::tasksLoaded(const QJsonObject &response) {
    QJsonArray tasksArray = response["tasks"].toArray();
    ui->tasksList->clear();

    for (const QJsonValue &task : tasksArray) {
        QJsonObject object = task.toObject();
        int id = object["id"].toInt();
        QString title = object["title"].toString();

        QString text = "№" + QString("%1. %2").arg(id).arg(title);
        QListWidgetItem *taskItem = new QListWidgetItem(text);
        taskItem->setData(Qt::UserRole, id);
        ui->tasksList->addItem(taskItem);
    }
}

void TaskPage::taskSelected(QListWidgetItem *taskItem) {
    currentTaskId = taskItem->data(Qt::UserRole).toInt();
    NetworkManager::instance()->loadTaskDetails(currentTaskId);
}

void TaskPage::taskDetailsLoaded(const QJsonObject &details) {
    int id = details["id"].toInt();
    QString title = details["title"].toString();
    QString description = details["description"].toString();
    QString difficulty = details["difficulty"].toString();

    QString text = "№" + QString::number(id) + ". " + title + ".\n";
    text += "Difficulty: " + difficulty + ".\n\n";
    text += description + "\n";
    ui->taskCondition->setText(text);
}

void TaskPage::clearClicked() {
    ui->codeEdit->clear();
}

void TaskPage::submitClicked() {
    if (currentTaskId == -1) {
        QMessageBox::warning(this, "Error", "Please choose the task to submit");
    }
    QString code = ui->codeEdit->toPlainText();
    if (code.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter the solution code");
    }
    ui->submitButton->setEnabled(false);
    ui->submitButton->setText("Sending...");
    NetworkManager::instance()->sendSolution(currentTaskId, code);
}

void TaskPage::solutionResult(const QJsonObject &result) {
    ui->submitButton->setEnabled(true);
    ui->submitButton->setText("Submit");
    QString status = result["status"].toString();

    if (status == "finished") {
        int passedTests = result["passed_tests"].toInt();
        int totalTests = result["total_tests"].toInt();
        QString verdict = result["verdict"].toString();
        if (verdict == "OK") {
            QMessageBox::information(this, "Completed", "All tests passed");
            ui->codeEdit->clear();
        } else {
            QString passedTestsText = "Tests passed: " + QString::number(passedTests) + "/" + QString::number(totalTests) + ".\n";
            QMessageBox::information(this, "Some mistakes", passedTestsText);
        }
    } else {
        QString compileError = result["compile_error"].toString();
        QMessageBox::warning(this, "Error!", compileError);
    }
}

void TaskPage::solutionError(const QString &message) {
    ui->submitButton->setEnabled(true);
    ui->submitButton->setText("Submit");
    QMessageBox::warning(this, "Error", message);
}
