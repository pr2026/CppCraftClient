#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>

class NetworkManager : public QObject {
    Q_OBJECT
public:
    static NetworkManager *instance();
    void login(const QString &username, const QString &password);
    void registration(const QString &username, const QString &password);
    void loadTasks();
    void loadTaskDetails(int taskId);
    void sendSolution(int taskId, const QString &code);

    void sendRequest(
        const QString &urlEnd,
        const QJsonObject &data,
        std::function<void(QNetworkReply *)> callback,
        const QString &method = "POST"
    );

private:
    explicit NetworkManager(QObject *parent = nullptr);
    static NetworkManager *m_instance;
    QNetworkAccessManager *manager;
    QString baseUrl = "http://172.16.124.64:8080";

signals:
    void loginSuccess();
    void registrationSuccess();
    void tasksLoadSuccess(const QJsonObject &response);
    void taskDetailsLoadSuccess(const QJsonObject &details);
    void solutionResult(const QJsonObject &result);
    void solutionError(const QString &message);
    void error(const QString &message);
};

#endif  // NETWORKMANAGER_H
