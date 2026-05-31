#ifndef TEACHERPAGE_H
#define TEACHERPAGE_H

#include <QWidget>

namespace Ui {
class teacherPage;
}

class teacherPage : public QWidget {
    Q_OBJECT

public:
    explicit teacherPage(QWidget *parent = nullptr);
    ~teacherPage();

private:
    Ui::teacherPage *ui;
};

#endif  // TEACHERPAGE_H
