#include "teacherpage.h"
#include "ui_teacherpage.h"
#include <QTimer>

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
    font.setFamily("Century Gothic");
    font.setPointSize(10);

    ui->taskList->setFont(font);

    ui->addButton->setFont(font);
    ui->editButton->setFont(font);
    ui->deleteButton->setFont(font);
    ui->createButton->setFont(font);
    ui->cancelButton->setFont(font);
    ui->statisticsButton->setFont(font);

    ui->nameLabel->setFont(font);
    ui->testsLabel->setFont(font);
    ui->difficultyLabe->setFont(font);
    ui->conditionLabel->setFont(font);

    ui->detailsTable->setFont(font);

    font.setPointSize(9);
    ui->tests->setFont(font);
    ui->taskCondition->setFont(font);
    ui->nameInput->setFont(font);

    font.setPointSize(11);
    ui->detailsTable->horizontalHeader()->setFont(font);
    ui->detailsTable->verticalHeader()->setFont(font);
    ui->generalStatisticsLabel->setFont(font);
    ui->detailsLabel->setFont(font);

    ui->taskList->setStyleSheet(
        "QListWidget::item {"
        "   background-color: #f4eee8"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #dbcbba;"
        "    color: black;"
        "}"
        "QListWidget::item:hover:!selected {"
        "    background-color: rgb(254,224,224);"
        // "    background-color: #e3d6ca;"m
        "}"
        "QListWidget:focus {"
        "    outline: none;"
        "}"
    );

    // loadTasks();

    ui->taskCondition->setStyleSheet("background-color: #f4eee8;");
    ui->tests->setStyleSheet("background-color: #f4eee8;");
    ui->testsEdit->setStyleSheet("background-color: #f4eee8;");
    ui->conditionEdit->setStyleSheet("background-color: #f4eee8;");

    ui->nameInput->setStyleSheet(
        "QLineEdit {"
        "    min-height: 25px;"
        "    max-height: 25px;"
        "    background-color: #f4eee8;"
        "}"
    );

    ui->difficultyBox->setStyleSheet(
        "QComboBox {"
        "    min-height: 25px;"
        "    max-height: 25px;"
        "    background-color: #f4eee8;"
        "}"
    );

    showViewMode();

    QString buttonStyle =
        "QPushButton {"
        "    min-height: 30px;"
        "    max-height: 30px;"
        "    background-color: rgb(254,224,224);"
        "}";

    ui->addButton->setStyleSheet(buttonStyle);
    ui->createButton->setStyleSheet(buttonStyle);
    ui->cancelButton->setStyleSheet(buttonStyle);
    ui->deleteButton->setStyleSheet(buttonStyle);
    ui->editButton->setStyleSheet(buttonStyle);
    ui->statisticsButton->setStyleSheet(buttonStyle);

    ui->detailsTable->verticalHeader()->setVisible(false);
    ui->detailsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->detailsTable->setStyleSheet(
        "QHeaderView::section {"
        "    font-size: 11pt;"
        "    font-weight: regular;"
        "    background-color: #f4eee8;"
        "}"
        );

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
        ui->statisticsButton, &QPushButton::clicked, this,
        &TeacherPage::statisticsClicked
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

    // TODO: ПОМЕНЯТЬ

    connect(NetworkManager::instance(), &NetworkManager::teacherStatisticsLoaded, this, &TeacherPage::statisticsLoaded);
    // QTimer::singleShot(100, this, [this]() {
    //     QJsonObject mockStats = createMockStatistics();
    //     statisticsLoaded(mockStats);
    // });
}

TeacherPage::~TeacherPage() {
    delete ui;
}

void TeacherPage::showViewMode() {
    isCreateMode = false;
    isStatisticsMode = false;
    ui->rightWidget->setCurrentWidget(ui->viewTaskPage);
    ui->addButton->setVisible(true);
    ui->statisticsButton->setText("See Statistics");
}

void TeacherPage::showCreateMode() {
    isCreateMode = true;
    isStatisticsMode = false;
    ui->rightWidget->setCurrentWidget(ui->createTaskPage);
    ui->addButton->setVisible(false);
    ui->taskList->setSelectionMode(QAbstractItemView::NoSelection);
    ui->statisticsButton->setText("See Statistics");
}

void TeacherPage::showStatisticsMode() {
    isStatisticsMode = true;
    isCreateMode = false;
    ui->rightWidget->setCurrentWidget(ui->statisticsPage);
    ui->addButton->setVisible(true);
    ui->statisticsButton->setText("Back to tasks");
}

void TeacherPage::loadTasks() {
    ui->taskList->clear();

    /*QVector<Task> tasksList = {
        {1, "Hello World", "Напишите программу, которая выведет 'Hello World'"},
        {2, "Сумма чисел", "На вход поступают 2 числа, найти их сумму"},
        {3, "Перевернуть вектор", "Переверните вектор"}};*/

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
    ui->taskList->clearSelection();
    ui->nameInput->clear();
    ui->conditionEdit->clear();
    ui->testsEdit->clear();
    ui->difficultyBox->setCurrentIndex(0);

    ui->createButton->setText("Create");
    showCreateMode();
    currentTaskId = -1;
}

void TeacherPage::statisticsClicked() {
    if (isStatisticsMode) {
        showViewMode();
    } else {
        showStatisticsMode();
    }
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
    ui->taskList->clearSelection();
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

void TeacherPage::statisticsLoaded(const QJsonObject &statistics) {
    // TODO: уточнить названия json

    // int totalAttemps = statistics["total_attempts"].toInt();
    // int successfulAttempts = statistics["solved_tasks"].toInt();
    // double successRate = statistics["success_rate"].toDouble();

    // QString generalText = QString("Total attempts: %1\n"
    //                           "Solved Tasks: %2\n"
    //                           "Success percentage: %3\%\n"
    //                           ).arg(totalAttemps).arg(successfulAttempts).arg(successRate,
    //                           0, 'f', 1);

    // ui->generalStatistics->setText(generalText);

    QJsonArray tasks = statistics["tasks_details"].toArray();

    ui->detailsTable->setRowCount(tasks.size());
    ui->detailsTable->setColumnCount(5);
    ui->detailsTable->setHorizontalHeaderLabels(
        {"ID", "Task", " Total attempts ", " Successful attempts ",
         "Success rate"}
    );

    for (int i = 0; i < tasks.size(); i++) {
        QJsonObject task = tasks[i].toObject();

        int taskId = task["task_id"].toInt();
        ui->detailsTable->setItem(
            i, 0, new QTableWidgetItem(QString::number(taskId))
        );
        ui->detailsTable->item(i, 0)->setFont(
            QFont("Century Gothic", 10, QFont::Bold)
        );

        ui->detailsTable->setItem(
            i, 1, new QTableWidgetItem(task["task_title"].toString())
        );

        int totalAttemps = task["total_attempts"].toInt();
        ui->detailsTable->setItem(
            i, 2, new QTableWidgetItem(QString::number(totalAttemps))
        );

        int successfulAttempts = task["successful_attempts"].toInt();
        ui->detailsTable->setItem(
            i, 3, new QTableWidgetItem(QString::number(successfulAttempts))
        );

        // TODO: double или нет...
        double successRate = task["success_rate"].toDouble();
        ui->detailsTable->setItem(
            i, 4, new QTableWidgetItem(QString::number(successRate))
        );
    }

    ui->detailsTable->resizeColumnsToContents();
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

QJsonObject TeacherPage::createMockStatistics() {
    QJsonObject stats;
    // stats["total_attempts"] = 15;
    // stats["solved_tasks"] = 8;
    // stats["success_rate"] = 53.3;

    QJsonArray tasks;

    QJsonObject task1;
    task1["task_id"] = 1;
    task1["task_title"] = "Hello World";
    task1["total_attempts"] = 10;
    task1["successful_attempts"] = 1;
    task1["success_rate"] = 10;
    tasks.append(task1);

    QJsonObject task2;
    task2["task_id"] = 2;
    task2["task_title"] = "Сумма чисел";
    task2["total_attempts"] = 5;
    task2["successful_attempts"] = 5;
    task2["success_rate"] = 100;
    tasks.append(task2);

    QJsonObject task3;
    task3["task_id"] = 4;
    task3["task_title"] = "Максимум из трёх";
    task3["total_attempts"] = 8;
    task3["successful_attempts"] = 4;
    task3["success_rate"] = 50;
    tasks.append(task3);

    stats["tasks_details"] = tasks;

    return stats;
}
