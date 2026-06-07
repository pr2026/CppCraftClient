#include <QApplication>
#include <QDir>
#include <QFile>
#include <QSslConfiguration>
#include <QSslSocket>
#include "mainwindow.h"
#include "networkmanager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
