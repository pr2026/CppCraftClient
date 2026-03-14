#ifndef TASKPAGE_H
#define TASKPAGE_H

#include <QVector>
#include <QWidget>
#include "task.h"

namespace Ui {
class TaskPage;
}

class TaskPage : public QWidget {
    Q_OBJECT

public:
    explicit TaskPage(QWidget *parent = nullptr);
    ~TaskPage();

private:
    Ui::TaskPage *ui;
    QVector<Task> tasksList;
    void loadTasks();

private slots:
    void showTask(int index);
    void clearClicked();
    void submitClicked();
};

#endif  // TASKPAGE_H
