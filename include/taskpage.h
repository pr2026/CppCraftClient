#ifndef TASKPAGE_H
#define TASKPAGE_H

#include <QVector>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QJsonArray>
#include <string>
#include "task.h"
#include "networkmanager.h"

namespace Ui {
class TaskPage;
}

class TaskPage : public QWidget {
    Q_OBJECT

public:
    explicit TaskPage(QWidget *parent = nullptr);
    ~TaskPage();
    void loadTasks();

private:
    Ui::TaskPage *ui;
    QVector<Task> tasksList;
    int currentTaskId = -1;

private slots:
    void tasksLoaded(const QJsonObject& tasks);
    void taskSelected(QListWidgetItem *taskItem);
    void taskDetailsLoaded(const QJsonObject &details);
    void clearClicked();
    void submitClicked();
    void solutionResult(const QJsonObject &result);
    void solutionError(const QString &message);
};

#endif  // TASKPAGE_H
