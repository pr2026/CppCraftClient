#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>
#include <QMessageBox>
#include "loginpage.h"
#include "taskpage.h"
#include "teacherpage.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LoginPage *loginPage;
    TaskPage *taskPage;
    TeacherPage *teacherPage;


private slots:
    void onLoginSuccess(const QString &role);
};

#endif  // MAINWINDOW_H
