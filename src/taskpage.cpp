#include "taskpage.h"
#include <QMessageBox>
#include <string>
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

    loadTasks();

    connect(
        ui->tasksList, &QListWidget::currentRowChanged, this,
        &TaskPage::showTask
    );
    connect(
        ui->clearButton, &QPushButton::clicked, this, &TaskPage::clearClicked
    );
    connect(
        ui->submitButton, &QPushButton::clicked, this, &TaskPage::submitClicked
    );
}

TaskPage::~TaskPage() {
    delete ui;
}

void TaskPage::loadTasks() {
    // sample tasks
    tasksList = {
        {1, "Hello World", "Напишите программу, которая выведет 'Hello World'"},
        {2, "Сумма чисел", "На вход поступают 2 числа, найти их сумму"},
        {3, "Перевернуть вектор", "Переверните вектор"}};

    for (const Task &task : tasksList) {
        QString text = "№" + QString::number(task.id) + ". " + task.title;
        ui->tasksList->addItem(text);
    }
}

void TaskPage::showTask(int index) {
    Task currentTask = tasksList[index];
    QString text = "№" + QString::number(currentTask.id) + ". " +
                   currentTask.title + ".\n\n";
    text += currentTask.description + ".\n";
    ui->taskCondition->setText(text);
}

void TaskPage::clearClicked() {
    ui->codeEdit->clear();
}

void TaskPage::submitClicked() {
    // sample
    QMessageBox::information(this, "Ура", "Ваш код отправлен на проверку!");
}
