#include "teacherpage.h"
#include "ui_teacherpage.h"

TeacherPage::TeacherPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::TeacherPage) {
    ui->setupUi(this);

    this->setContentsMargins(0, 0, 0, 0);
    ui->rightWidget->setContentsMargins(0, 0, 0, 0);
    this->setStyleSheet("background-color: #ffffff;");

    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 1);
    ui->splitter->setSizes({400, width() - 400});

    ui->taskCondition->setWordWrapMode(QTextOption::WordWrap);

    ui->taskList->setUniformItemSizes(true);
    ui->taskList->setGridSize(QSize(200, 35));

    QFont font;
    font.setFamily("Consolas");
    font.setPointSize(10);
    ui->taskList->setFont(font);
    ui->taskCondition->setFont(font);
    ui->addButton->setFont(font);
    ui->editButton->setFont(font);
    ui->deleteButton->setFont(font);
    ui->createButton->setFont(font);
    ui->cancelButton->setFont(font);

    ui->taskList->setStyleSheet(
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

    showViewMode();
    loadTasks();

    connect(
        NetworkManager::instance(), &NetworkManager::tasksLoadSuccess, this,
        &TeacherPage::tasksLoaded
        );

    connect(
        NetworkManager::instance(), &NetworkManager::taskDetailsLoadSuccess, this,
        &TeacherPage::taskDetailsLoaded
        );

    connect(
        ui->taskList, &QListWidget::itemClicked, this,
        &TeacherPage::taskSelected
        );

    connect(
        ui->addButton, &QPushButton::clicked, this, &TeacherPage::addTaskClicked
        );

    connect(ui->deleteButton, &QPushButton::clicked, this, &TeacherPage::deleteTaskClicked);

    connect(ui->editButton, &QPushButton::clicked, this, &TeacherPage::editTaskClicked);

    connect(
        ui->createButton, &QPushButton::clicked, this, &TeacherPage::createTaskClicked
        );

    connect(ui->cancelButton, &QPushButton::clicked, this, &TeacherPage::calcelCreatingButton);

}

TeacherPage::~TeacherPage() {
    delete ui;
}

void TeacherPage::showViewMode() {
    isCreateMode = false;
    ui->rightWidget->setCurrentWidget(ui->viewTaskPage);
    ui->addButton->setVisible(true);
}

void TeacherPage::showCreateMode() {
    isCreateMode = true;
    ui->rightWidget->setCurrentWidget(ui->createTaskPage);
    ui->addButton->setVisible(false);
}

void TeacherPage::loadTasks() {
    ui->taskList->clear();
    QVector<Task> tasksList = {
                 {1, "Hello World", "Напишите программу, которая выведет 'Hello World'"},
                 {2, "Сумма чисел", "На вход поступают 2 числа, найти их сумму"},
                 {3, "Перевернуть вектор", "Переверните вектор"}};

    for (const Task &task : tasksList) {
        QString text = "№" + QString::number(task.id) + ". " + task.title;
        ui->taskList->addItem(text);
    }

    // NetworkManager::instance()->loadTasks();
}

void TeacherPage::tasksLoaded(const QJsonObject &response) {
    QJsonArray tasksArray = response["tasks"].toArray();
    ui->taskList->clear();

    for (const QJsonValue &task : tasksArray) {
        QJsonObject object = task.toObject();
        int id = object["id"].toInt();
        QString title = object["title"].toString();

        QString text = "№" + QString("%1. %2").arg(id).arg(title);
        QListWidgetItem *taskItem = new QListWidgetItem(text);
        taskItem->setData(Qt::UserRole, id);
        ui->taskList->addItem(taskItem);
    }
}

void TeacherPage::taskSelected(QListWidgetItem *taskItem) {
    if (isCreateMode) return;
    currentTaskId = taskItem->data(Qt::UserRole).toInt();
    NetworkManager::instance()->loadTaskDetails(currentTaskId);
}

void TeacherPage::taskDetailsLoaded(const QJsonObject &details) {
    int id = details["id"].toInt();
    QString title = details["title"].toString();
    QString description = details["description"].toString();
    QString difficulty = details["difficulty"].toString();

    QString text = "№" + QString::number(id) + ". " + title + ".\n";
    text += "Difficulty: " + difficulty + ".\n\n";
    text += description + "\n";
    ui->taskCondition->setText(text);
}

void TeacherPage::addTaskClicked() {
    showCreateMode();
    currentTaskId = -1;
}

void TeacherPage::deleteTaskClicked(int taskId) {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Deleting", "Are you sure you want to delete this task?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // TODO: добавить этот метод

        // NetworkManager::instance()->deleteTask(task);
        loadTasks();
    }
}

void TeacherPage::editTaskClicked(int taskId) {
    currentTaskId = taskId;
    NetworkManager::instance()->loadTaskDetails(taskId);
    showCreateMode();
    ui->createButton->setText("Save");
    connect(NetworkManager::instance(), &NetworkManager::taskDetailsLoadSuccess, this, &TeacherPage::fillEditForm, Qt::SingleShotConnection);
    ui->createButton->setText("Create");
}

void TeacherPage::fillEditForm(const QJsonObject &details) {

    // TODO: чекнуть как это у Яры называется
    ui->nameInput->setText(details["title"].toString());
    ui->difficultyBox->setCurrentText(details["difficulty"].toString());
    ui->conditionEdit->setText((details["condition"].toString()));
    ui->tests->setText(details["tests"].toString());
}

void TeacherPage::createTaskClicked() {
    QString taskName = ui->nameInput->text();
    QString difficulty = ui->difficultyBox->currentText();
    QString condition = ui->conditionEdit->toPlainText();
    QString tests = ui->testsEdit->toPlainText();  // изменить!!!

    if (taskName.isEmpty() || condition.isEmpty()) {
        QMessageBox::warning(this, "Error", "Input the task name and condition");
        return;
    }

    // TODO: проверить как это все называется на сервере у Яры
    QJsonObject task;
    task["title"] = taskName;
    task["difficulty"] = difficulty;
    task["condition"] = condition;
    task["tests"] = tests;


    // TODO: сделать эти методы для NetworkManager
    if (currentTaskId == -1) {
        // NetworkManager::instance()->createTask(task);
    } else {
        // NetworkManager::instance()->editTask(currentTaskId, task);
    }

    showViewMode();
    loadTasks();
}

void TeacherPage::calcelCreatingButton() {
    showViewMode();
}
