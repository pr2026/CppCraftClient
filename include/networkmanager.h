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
    void registration(const QString &username, const QString &password, const QString &role);
    void loadTasks();
    void loadTaskDetails(int taskId);
    void sendSolution(int taskId, const QString &code);

    void createTask(const QJsonObject &task);
    void editTask(int taskId, const QJsonObject &taskData);
    void deleteTask(int taskId);

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
    QString baseUrl = "http://172.16.125.139:8080";

signals:
    // Student TaskPage signals
    void loginSuccess(const QString &role);
    void registrationSuccess(const QString &role);
    void tasksLoadSuccess(const QJsonObject &response);
    void taskDetailsLoadSuccess(const QJsonObject &details);
    void solutionResult(const QJsonObject &result);
    void solutionError(const QString &message);

    // Teaches's page signals
    void taskCreated();
    void taskEdited();
    void taskDeleted();

    // common
    void error(const QString &message);

};

#endif  // NETWORKMANAGER_H
