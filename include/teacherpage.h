#ifndef TEACHERPAGE_H
#define TEACHERPAGE_H

#include <QJsonArray>
#include <QJsonObject>
#include <QListWidget>
#include <QMessageBox>
#include <QWidget>
#include "networkmanager.h"
#include "task.h"

namespace Ui {
class TeacherPage;
}

class TeacherPage : public QWidget {
    Q_OBJECT

public:
    explicit TeacherPage(QWidget *parent = nullptr);
    ~TeacherPage();
    void loadTasks();

private:
    Ui::TeacherPage *ui;
    int currentTaskId = -1;
    bool isCreateMode = false;

    void showViewMode();
    void showCreateMode();
    void fillEditForm(const QJsonObject &details);
    void clearTaskForm();
    QJsonArray parseTests(const QString &testsText);
    void showTests(const QJsonArray &tests);

private slots:
    void tasksLoaded(const QJsonObject &tasks);
    void taskDetailsLoaded(const QJsonObject &details);
    void taskSelected(QListWidgetItem *taskItem);

    void addTaskClicked();
    void editTaskClicked();
    void deleteTaskClicked();
    void createTaskClicked();
    void calcelCreatingButton();

    void taskCreated(int taskId);
    void taskEdited();
    void taskDeleted();
};

#endif  // TEACHERPAGE_H
