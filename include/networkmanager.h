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
    void registration(
        const QString &username,
        const QString &password,
        const QString &role
    );
    void loadTasks();
    void loadTaskDetails(int taskId);
    void sendSolution(int taskId, const QString &code);
    void loadStudentStatistics();
    void loadTeacherStatistics();

    void createTask(const QJsonObject &task);
    void editTask(int taskId, const QJsonObject &taskData);
    void deleteTask(int taskId);

    void logout();

    void sendRequest(
        const QString &urlEnd,
        const QJsonObject &data,
        std::function<void(QNetworkReply *)> callback,
        const QString &method = "POST"
    );

    void
    setCurrentUser(const QString &userName, const QString &role, int userId);
    QString getRole();
    QString getUserName();
    int getUserId();

private:
    explicit NetworkManager(QObject *parent = nullptr);
    static NetworkManager *m_instance;
    QNetworkAccessManager *manager;
    QString baseUrl = "http://172.20.156.94:8080";

    QString currentUserName;
    QString currentUserRole;
    int currentUserId;

signals:
    // Student TaskPage signals
    void loginSuccess(const QString &userName, const QString &role, int userId);
    void registrationSuccess(
        const QString &userName,
        const QString &role,
        int userId
    );
    void tasksLoadSuccess(const QJsonObject &response);
    void taskDetailsLoadSuccess(const QJsonObject &details);
    void solutionResult(const QJsonObject &result);
    void solutionError(const QString &message);
    void studentStatisticsLoaded(const QJsonObject &statistics);

    // Teaches's page signals
    void taskCreated(int taskId);
    void taskEdited();
    void taskDeleted();
    void teacherStatisticsLoaded(const QJsonObject &statistics);

    // common
    void error(const QString &message);
};

#endif  // NETWORKMANAGER_H
