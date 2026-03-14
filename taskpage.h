#ifndef TASKPAGE_H
#define TASKPAGE_H

#include <QWidget>
#include <QVector>
#include "task.h"

namespace Ui {
class TaskPage;
}

class TaskPage : public QWidget
{
    Q_OBJECT

public:
    explicit TaskPage(QWidget *parent = nullptr);
    ~TaskPage();

private:
    Ui::TaskPage *ui;
    QVector<Task> tasks_list;
    void load_tasks();

private slots:
    void show_task(int index);
    void clear_clicked();
};

#endif // TASKPAGE_H
