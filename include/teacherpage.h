#ifndef TEACHERPAGE_H
#define TEACHERPAGE_H

#include "networkmanager.h"
#include "task.h"
#include <QWidget>
#include <QListWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

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
    QJsonArray parseTests(const QString &testsText);
    void showTests(const QJsonArray& tests);

private slots:
    void tasksLoaded(const QJsonObject& tasks);
    void taskDetailsLoaded(const QJsonObject &details);
    void taskSelected(QListWidgetItem *taskItem);

    void addTaskClicked();
    void editTaskClicked(int taskId);
    void deleteTaskClicked(int taskId);
    void createTaskClicked();
    void calcelCreatingButton();

};

#endif  // TEACHERPAGE_H
