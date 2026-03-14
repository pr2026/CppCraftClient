#include "taskpage.h"
#include "ui_taskpage.h"
#include <string>

TaskPage::TaskPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TaskPage)
{
    ui->setupUi(this);/*
    ui->splitter_2->setSizes({245, this->width() - 250});

    ui->splitter->setSizes({145, this->height() - 150});*/
    ui->taskCondition->setWordWrapMode(QTextOption::WordWrap);

    load_tasks();

    connect(ui->tasksList, &QListWidget::currentRowChanged,
            this, &TaskPage::show_task);

    connect(ui->clearButton, &QPushButton::clicked, this, &TaskPage::clear_clicked);
}

TaskPage::~TaskPage()
{
    delete ui;
}

void TaskPage::load_tasks()
{
    tasks_list = {{1, "Hello World", "Напишите программу, которая выведет 'Hello World'"},
                  {2, "Сумма чисел", "На вход поступают 2 числа, найти их сумму"},
                  {3, "Перевернуть вектор", "Переверните векторffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff\ndhvhbeviweh\nvnkjdfnxvoenjkbvf\nvjdfkbefkdnb\n"}};

    for (const Task &task : tasks_list) {
        QString text = "№" + QString::number(task.id) + ". " + task.title;
        ui->tasksList->addItem(text);
    }
}

void TaskPage::show_task(int index)
{
    Task current_task = tasks_list[index];
    QString text = "№" + QString::number(current_task.id) + ". " + current_task.title + ".\n\n";
    text += current_task.description + ".\n";
    ui->taskCondition->setText(text);
}

void TaskPage::clear_clicked()
{
    ui->codeEdit->clear();
}
