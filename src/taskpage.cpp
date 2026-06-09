#include "taskpage.h"
#include "ui_taskpage.h"

TaskPage::TaskPage(QWidget *parent) : QWidget(parent), ui(new Ui::TaskPage) {
    ui->setupUi(this);

    this->setContentsMargins(0, 0, 0, 0);
    ui->rightWidget->setContentsMargins(0, 0, 0, 0);
    this->setStyleSheet("background-color: #ffffff;");

    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 1);
    ui->splitter->setSizes({400, width() - 400});

    ui->taskCondition->setWordWrapMode(QTextOption::WordWrap);

    ui->tasksList->setUniformItemSizes(true);
    ui->tasksList->setGridSize(QSize(200, 35));

    QFont font;
    font.setFamily("Century Gothic");
    font.setPointSize(10);
    ui->tasksList->setFont(font);
    ui->taskCondition->setFont(font);
    ui->clearButton->setFont(font);
    ui->submitButton->setFont(font);
    ui->statisticsButton->setFont(font);
    ui->detailsTable->setFont(font);
    ui->generalStatistics->setFont(font);

    font.setPointSize(11);
    ui->detailsTable->horizontalHeader()->setFont(font);
    ui->detailsTable->verticalHeader()->setFont(font);
    ui->generalStatisticsLabel->setFont(font);
    ui->detailsLabel->setFont(font);

    ui->detailsTable->setStyleSheet(
        "QHeaderView::section {"
        "    font-size: 11pt;"
        "    font-weight: regular;"
        "    background-color: #f4eee8;"
        "}"
        );

    QString buttonStyle =
        "QPushButton {"
        "    min-height: 35px;"
        "    max-height: 35px;"
        "    background-color: rgb(254,224,224);"
        "}";

    ui->statisticsButton->setStyleSheet(buttonStyle);
    ui->detailsTable->verticalHeader()->setVisible(false);

    ui->tasksList->setStyleSheet(
        "QListWidget::item {"
        "   background-color: #f4eee8"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #dbcbba;"
        "    color: black;"
        "}"
        "QListWidget::item:hover:!selected {"
        "    background-color: rgb(254,224,224);"
        // "    background-color: #e3d6ca;"
        "}"
        "QListWidget:focus {"
        "    outline: none;"
        "}"
        );

    loadTasks();

    codeEditor = new QsciScintilla(this);
    QsciLexerCPP *lexer = new QsciLexerCPP(this);
    codeEditor->setLexer(lexer);

    codeEditorSetter(lexer);

    ui->splitter_2->insertWidget(1, codeEditor);
    ui->splitter_2->setStretchFactor(0, 1);
    ui->splitter_2->setStretchFactor(1, 2);
    ui->splitter_2->setSizes({100, height() - 130});

    ui->taskCondition->setStyleSheet("background-color: #f4eee8;");
    ui->submitButton->setStyleSheet("background-color: rgb(254,224,224);");
    ui->clearButton->setStyleSheet("background-color: rgb(254,224,224);");

    ui->detailsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(
        NetworkManager::instance(), &NetworkManager::tasksLoadSuccess, this,
        &TaskPage::tasksLoaded
    );
    connect(
        NetworkManager::instance(), &NetworkManager::taskDetailsLoadSuccess,
        this, &TaskPage::taskDetailsLoaded
    );
    connect(
        ui->tasksList, &QListWidget::itemClicked, this, &TaskPage::taskSelected
    );
    connect(
        ui->clearButton, &QPushButton::clicked, this, &TaskPage::clearClicked
    );
    connect(
        ui->submitButton, &QPushButton::clicked, this, &TaskPage::submitClicked
    );
    connect(ui->statisticsButton, &QPushButton::clicked, this, &TaskPage::switchMode);

    connect(
        NetworkManager::instance(), &NetworkManager::solutionResult, this,
        &TaskPage::solutionResult
    );
    connect(
        NetworkManager::instance(), &NetworkManager::solutionError, this,
        &TaskPage::solutionError
    );

    // TODO: ПОМЕНЯТЬ

    // connect(NetworkManager::instance(), &NetworkManager::studentStatisticsLoaded, this, &TaskPage::statisticsLoaded);
    QTimer::singleShot(100, this, [this]() {
        QJsonObject mockStats = createMockStatistics();
        statisticsLoaded(mockStats);
    });
}

TaskPage::~TaskPage() {
    delete ui;
}

void TaskPage::codeEditorSetter(QsciLexerCPP *lexer) {
    codeEditor->setUtf8(true);

    codeEditor->setMarginLineNumbers(1, true);
    codeEditor->setMarginWidth(1, 25);
    codeEditor->setMarginsBackgroundColor(QColor("#e0dad6"));

    codeEditor->setCaretLineVisible(true);
    codeEditor->setCaretLineBackgroundColor(QColor("#f4dede"));
    codeEditor->setAutoIndent(true);
    codeEditor->setTabWidth(4);

    QFont codeEditorFont;
    codeEditorFont.setFamily("Consolas");
    codeEditorFont.setPointSize(11);
    lexer->setFont(codeEditorFont);
    codeEditor->setMarginsFont(codeEditorFont);

    lexer->setPaper(QColor("#f4eee8"));
    codeEditor->setColor(QColor("#615f5f"));

    lexer->setColor(QColor("#a49e9e"), QsciLexerCPP::Comment);
    lexer->setColor(QColor("#a49e9e"), QsciLexerCPP::CommentLine);

    lexer->setColor(QColor("#5F4B8B"), QsciLexerCPP::Keyword);
    lexer->setColor(QColor("#5F4B8B"), QsciLexerCPP::KeywordSet2);
    lexer->setColor(QColor("#5F4B8B"), QsciLexerCPP::Operator);

    lexer->setColor(QColor("#CA4286"), QsciLexerCPP::GlobalClass);

    lexer->setColor(QColor("#009473"), QsciLexerCPP::Number);

    lexer->setColor(QColor("#88B04B"), QsciLexerCPP::DoubleQuotedString);
    lexer->setColor(QColor("#45B5AA"), QsciLexerCPP::SingleQuotedString);

    lexer->setColor(QColor("#0F4C81"), QsciLexerCPP::PreProcessor);

    lexer->setColor(QColor("#D94F70"), QsciLexerCPP::Identifier);
}

void TaskPage::loadTasks() {
    // sample tasks
    tasksList = {
                 {1, "Hello World", "Напишите программу, которая выведет'Hello World'"},
        {2, "Сумма чисел", "На вход поступают 2 числа, найти их сумму"},
        {3, "Перевернуть вектор", "Переверните вектор"}};

    for (const Task &task : tasksList) {
        QString text = "№" + QString::number(task.id) + ". " + task.title;
        ui->tasksList->addItem(text);
    }

    // NetworkManager::instance()->loadTasks();
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
    codeEditor->clear();
}

void TaskPage::submitClicked() {
    if (currentTaskId == -1) {
        QMessageBox::warning(this, "Error", "Please choose the task to submit");
    }
    QString code = codeEditor->text();
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
            codeEditor->clear();
        } else {
            QString passedTestsText =
                "Tests passed: " + QString::number(passedTests) + "/" +
                QString::number(totalTests) + ".\n";
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

void TaskPage::switchMode() {
    if (isCodingMode == true) {
        showStatisticsMode();
    } else {
        showCodingMode();
    }
}

void TaskPage::showStatisticsMode() {
    isCodingMode = false;
    ui->rightWidget->setCurrentWidget(ui->statisticsPage);
    ui->statisticsButton->setText("Back to tasks");
    ui->tasksList->clearSelection();
    ui->tasksList->setSelectionMode(QAbstractItemView::NoSelection);

    NetworkManager::instance()->loadStudentStatistics();
}

void TaskPage::showCodingMode() {
    isCodingMode = true;
    ui->rightWidget->setCurrentWidget(ui->codingPage);
    ui->statisticsButton->setText("See Statistics");

    NetworkManager::instance()->loadTasks();
}

void TaskPage::statisticsLoaded(const QJsonObject& statistics) {

    int totalAttemps = statistics["total_attempts"].toInt();
    int successfulAttempts = statistics["solved_tasks"].toInt();
    double successRate = statistics["success_rate"].toDouble();

    QString generalText = QString("Total attempts: %1\n"
                                  "Solved Tasks: %2\n"
                              "Success percentage: %3\%\n"
                                  ).arg(totalAttemps).arg(successfulAttempts).arg(successRate, 0, 'f', 1);

    ui->generalStatistics->setText(generalText);

    QJsonArray tasks = statistics["tasks_details"].toArray();

    ui->detailsTable->setRowCount(tasks.size());
    ui->detailsTable->setColumnCount(5);
    ui->detailsTable->setHorizontalHeaderLabels({
        "ID", "Task", " Max successful tests ", "Attempts amount", "Status"
    });

    for (int i = 0; i < tasks.size(); i++) {
        QJsonObject task = tasks[i].toObject();

        int taskId = task["task_id"].toInt();
        ui->detailsTable->setItem(i, 0, new QTableWidgetItem(QString::number(taskId)));
        ui->detailsTable->item(i, 0)->setFont(QFont("Century Gothic", 10, QFont::Bold));

        ui->detailsTable->setItem(i, 1, new QTableWidgetItem(task["task_title"].toString()));

        int bestResult = task["best_result"].toInt();
        ui->detailsTable->setItem(i, 2, new QTableWidgetItem(QString::number(bestResult)));

        int totalAttemps = task["attempts"].toInt();
        ui->detailsTable->setItem(i, 3, new QTableWidgetItem(QString::number(totalAttemps)));


        bool solved = task["is_solved"].toBool();
        ui->detailsTable->setItem(i, 4, new QTableWidgetItem(solved ? "✅" : "❌"));
    }

    ui->detailsTable->resizeColumnsToContents();
}

// TODO: УДАЛИТЬ
QJsonObject TaskPage::createMockStatistics()
{
    QJsonObject stats;
    stats["total_attempts"] = 15;
    stats["solved_tasks"] = 8;
    stats["success_rate"] = 53.3;

    QJsonArray tasks;

    QJsonObject task1;
    task1["task_id"] = 1;
    task1["task_title"] = "Hello World";
    task1["is_solved"] = true;
    task1["best_result"] = 1;
    task1["attempts"] = 3;
    tasks.append(task1);

    QJsonObject task2;
    task2["task_id"] = 2;
    task2["task_title"] = "Сумма чисел";
    task2["is_solved"] = false;
    task2["best_result"] = 2;
    task2["attempts"] = 5;
    tasks.append(task2);

    QJsonObject task3;
    task3["task_id"] = 4;
    task3["task_title"] = "Максимум из трёх";
    task3["is_solved"] = false;
    task3["best_result"] = 1;
    task3["attempts"] = 7;
    tasks.append(task3);

    stats["tasks_details"] = tasks;

    return stats;
}
