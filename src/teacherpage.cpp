#include "teacherpage.h"
#include "ui_teacherpage.h"

teacherPage::teacherPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::teacherPage) {
    ui->setupUi(this);
}

teacherPage::~teacherPage() {
    delete ui;
}
