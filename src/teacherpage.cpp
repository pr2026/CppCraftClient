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

    QString buttonStyle =
        "QPushButton {"
        "    min-height: 25px;"
        "    max-height: 25px;"
        "    background-color: rgb(254,224,224);"
        "}";

    ui->addButton->setStyleSheet(buttonStyle);
    ui->createButton->setStyleSheet(buttonStyle);
    ui->cancelButton->setStyleSheet(buttonStyle);
    ui->deleteButton->setStyleSheet(buttonStyle);
    ui->editButton->setStyleSheet(buttonStyle);

    connect(
        NetworkManager::instance(), &NetworkManager::tasksLoadSuccess, this,
        &TeacherPage::tasksLoaded
    );

    connect(
        NetworkManager::instance(), &NetworkManager::taskDetailsLoadSuccess,
        this, &TeacherPage::taskDetailsLoaded
    );

    connect(
        ui->taskList, &QListWidget::itemClicked, this,
        &TeacherPage::taskSelected
    );

    connect(
        ui->addButton, &QPushButton::clicked, this, &TeacherPage::addTaskClicked
    );

    connect(
        ui->deleteButton, &QPushButton::clicked, this,
        &TeacherPage::deleteTaskClicked
    );

    connect(
        ui->editButton, &QPushButton::clicked, this,
        &TeacherPage::editTaskClicked
    );

    connect(
        ui->createButton, &QPushButton::clicked, this,
        &TeacherPage::createTaskClicked
    );

    connect(
        ui->cancelButton, &QPushButton::clicked, this,
        &TeacherPage::calcelCreatingButton
    );

    connect(
        NetworkManager::instance(), &NetworkManager::taskCreated, this,
        &TeacherPage::taskCreated, Qt::UniqueConnection
    );

    connect(
        NetworkManager::instance(), &NetworkManager::taskDeleted, this,
        &TeacherPage::taskDeleted, Qt::UniqueConnection
    );

    connect(
        NetworkManager::instance(), &NetworkManager::taskEdited, this,
        &TeacherPage::taskEdited, Qt::UniqueConnection
    );
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
    // ui->taskList->clear();

    // QVector<Task> tasksList = {
    //              {1, "Hello World", "Напишите программу, которая выведет
    //              'Hello World'"}, {2, "Сумма чисел", "На вход поступают 2
    //              числа, найти их сумму"}, {3, "Перевернуть вектор",
    //              "Переверните вектор"}};

    // for (const Task &task : tasksList) {
    //     QString text = "№" + QString::number(task.id) + ". " + task.title;
    //     ui->taskList->addItem(text);
    // }

    NetworkManager::instance()->loadTasks();
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
    if (isCreateMode) {
        return;
    }
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

    showTests(details["tests"].toArray());
}

void TeacherPage::showTests(const QJsonArray &tests) {
    if (tests.isEmpty()) {
        ui->tests->setText("no tests have been added yet");
        return;
    }

    QString testsText;

    for (const QJsonValue &value : tests) {
        QJsonObject test = value.toObject();
        QString input = test["input"].toString();
        QString output = test["expected_output"].toString();

        testsText += "input: " + input + "\n";
        testsText += "output: " + output + "\n\n";
    }

    ui->tests->setText(testsText);
}

void TeacherPage::addTaskClicked() {
    ui->nameInput->clear();
    ui->conditionEdit->clear();
    ui->testsEdit->clear();
    ui->difficultyBox->setCurrentIndex(0);

    ui->createButton->setText("Create");
    showCreateMode();
    currentTaskId = -1;
}

void TeacherPage::deleteTaskClicked() {
    if (currentTaskId <= 0) {
        QMessageBox::warning(this, "Error", "Choose the task to edit");
    }
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Deleting", "Are you sure you want to delete this task?",
        QMessageBox::Yes | QMessageBox::No
    );
    if (reply == QMessageBox::Yes) {
        NetworkManager::instance()->deleteTask(currentTaskId);
    }
}

void TeacherPage::editTaskClicked() {
    if (currentTaskId < 0) {
        QMessageBox::warning(this, "Error", "Choose the task to edit");
    }
    NetworkManager::instance()->loadTaskDetails(currentTaskId);
    showCreateMode();
    ui->createButton->setText("Save");
    connect(
        NetworkManager::instance(), &NetworkManager::taskDetailsLoadSuccess,
        this, &TeacherPage::fillEditForm, Qt::SingleShotConnection
    );
}

void TeacherPage::fillEditForm(const QJsonObject &details) {
    ui->nameInput->setText(details["title"].toString());
    ui->difficultyBox->setCurrentText(details["difficulty"].toString());
    ui->conditionEdit->setText((details["description"].toString()));

    QJsonArray testsArray = details["tests"].toArray();
    QString testsText;
    for (const auto &test : testsArray) {
        QJsonObject t = test.toObject();
        testsText += t["input"].toString() + " : " +
                     t["expected_output"].toString() + "\n";
    }
    ui->tests->setPlainText(testsText);
}

void TeacherPage::createTaskClicked() {
    QString taskName = ui->nameInput->text();
    QString difficulty = ui->difficultyBox->currentText();
    QString condition = ui->conditionEdit->toPlainText();
    QString testsText = ui->testsEdit->toPlainText();
    QJsonArray tests = parseTests(testsText);

    if (taskName.isEmpty() || condition.isEmpty()) {
        QMessageBox::warning(
            this, "Error", "Input the task name and condition"
        );
        return;
    }

    QJsonObject task;
    task["title"] = taskName;
    task["difficulty"] = difficulty;
    task["description"] = condition;
    task["tests"] = tests;

    if (currentTaskId == -1) {
        NetworkManager::instance()->createTask(task);
    } else {
        NetworkManager::instance()->editTask(currentTaskId, task);
    }
}

QJsonArray TeacherPage::parseTests(const QString &testsText) {
    QJsonArray testsArray;

    QJsonDocument document = QJsonDocument::fromJson(testsText.toUtf8());
    if (!document.isNull() && document.isArray()) {
        return document.array();
    }

    QStringList lines = testsText.split('\n', Qt::SkipEmptyParts);
    for (const QString &line : lines) {
        QStringList divided = line.split(":");
        if (divided.size() == 2) {
            QJsonObject test;
            test["input"] = divided[0].trimmed();
            test["expected_output"] = divided[1].trimmed();
            testsArray.append(test);
        }
    }

    return testsArray;
}

void TeacherPage::calcelCreatingButton() {
    showViewMode();
}

void TeacherPage::taskCreated(int taskId) {
    loadTasks();
    showViewMode();
    NetworkManager::instance()->loadTaskDetails(taskId);
}

void TeacherPage::taskDeleted() {
    currentTaskId = -1;
    loadTasks();
    showViewMode();
    ui->taskList->clearSelection();
}

void TeacherPage::taskEdited() {
    loadTasks();
    showViewMode();
    NetworkManager::instance()->loadTaskDetails(currentTaskId);
}
