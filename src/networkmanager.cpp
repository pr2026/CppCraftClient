#include "../include/networkmanager.h"
#include <QSslConfiguration>
#include <QSslSocket>

NetworkManager *NetworkManager::m_instance = nullptr;

NetworkManager *NetworkManager::instance() {
    if (!m_instance) {
        m_instance = new NetworkManager();
    }
    return m_instance;
}

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent), manager(new QNetworkAccessManager(this)) {
    // QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    // config.setPeerVerifyMode(QSslSocket::VerifyNone);
    // config.setProtocol(QSsl::AnyProtocol);  // разрешить любые протоколы
    // QSslConfiguration::setDefaultConfiguration(config);

    // connect(
    //     manager, &QNetworkAccessManager::sslErrors, this,
    //     [](QNetworkReply *reply, const QList<QSslError> &errors) {
    //         for (const auto &error : errors) {
    //             qDebug() << "SSL error:" << error.errorString();
    //         }
    //         reply->ignoreSslErrors();
    //     }
    // );
}

void NetworkManager::login(const QString &username, const QString &password) {
    QJsonObject loginData;
    loginData["username"] = username;
    loginData["password"] = password;

    sendRequest(
        "/login", loginData,
        [this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QJsonObject jsonObj =
                    QJsonDocument::fromJson(reply->readAll()).object();
                // qDebug() << "Login response:" << reply->readAll();
                QString role = jsonObj["role"].toString();
                QString userName = jsonObj["username"].toString();
                int userId = jsonObj["userId"].toInt();
                qDebug() << userId;
                emit loginSuccess(userName, role, userId);
            } else {
                emit error("Cannot login: " + reply->errorString());
            }
            reply->deleteLater();
        },
        "POST"
    );
}

void NetworkManager::registration(
    const QString &username,
    const QString &password,
    const QString &role
) {
    QJsonObject registrationData;
    registrationData["username"] = username;
    registrationData["password"] = password;
    registrationData["role"] = role;

    sendRequest(
        "/registration", registrationData,
        [this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                // qDebug() << "Login response:" << reply->readAll();
                QJsonObject jsonObj =
                    QJsonDocument::fromJson(reply->readAll()).object();
                QString userRole = jsonObj["role"].toString();
                QString userName = jsonObj["username"].toString();
                int userId = jsonObj["userId"].toInt();
                emit registrationSuccess(userName, userRole, userId);
            } else {
                emit error("Failed to registrate: " + reply->errorString());
            }
            reply->deleteLater();
        },
        "POST"
    );
}

void NetworkManager::loadTasks() {
    sendRequest(
        "/tasks", {},
        [this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QJsonObject jsonObj =
                    QJsonDocument::fromJson(reply->readAll()).object();
                emit tasksLoadSuccess(jsonObj);
            } else {
                emit error("Failed to login: " + reply->errorString());
            }
            reply->deleteLater();
        },
        "GET"
    );
}

void NetworkManager::loadTaskDetails(int taskId) {
    QString url = "/tasks/" + QString::number(taskId);
    sendRequest(
        url, {},
        [this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QJsonObject jsonObj =
                    QJsonDocument::fromJson(reply->readAll()).object();
                emit taskDetailsLoadSuccess(jsonObj);
            } else {
                emit error(
                    "Failed to load task details: " + reply->errorString()
                );
            }
            reply->deleteLater();
        },
        "GET"
    );
}

void NetworkManager::sendSolution(int taskId, const QString &code) {
    QJsonObject submissionData;
    submissionData["task_id"] = taskId;
    submissionData["code"] = code;

    QString url = "/tasks/" + QString::number(taskId) + "/submit";
    sendRequest(
        url, submissionData,
        [this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                // can be changed after debugging

                QByteArray responseData = reply->readAll();
                qDebug() << "Ответ сервера:" << responseData;

                QJsonDocument document = QJsonDocument::fromJson(responseData);
                if (document.isNull()) {
                    emit solutionError("Server returned wrong format");
                    return;
                }
                QJsonObject jsonObj = document.object();

                emit solutionResult(jsonObj);
            } else {
                emit solutionError(
                    "Failed to send solution: " + reply->errorString()
                );
            }
            reply->deleteLater();
        },
        "POST"
    );
}

void NetworkManager::loadStudentStatistics() {
    // TODO: уточнить эндпойнт
    QString url = "/students/" + QString::number(currentUserId) + "/statistics";

    // TODO: уточнить нужно ли что-то передавать в json
    sendRequest(
        url, {},
        [this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QJsonObject jsonObj =
                    QJsonDocument::fromJson(reply->readAll()).object();
                emit studentStatisticsLoaded(jsonObj);
            } else {
                emit error(
                    "Failed to load statistics: " + reply->errorString()
                );
            }
            reply->deleteLater();
        },
        "GET"
    );
}

void NetworkManager::loadTeacherStatistics() {
    // TODO: уточнить эндпойнт
    QString url = "/teachers/" + QString::number(currentUserId) + "/statistics";

    // TODO: уточнить нужно ли что-то передавать в json
    sendRequest(
        url, {},
        [this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QJsonObject jsonObj =
                    QJsonDocument::fromJson(reply->readAll()).object();
                emit teacherStatisticsLoaded(jsonObj);
            } else {
                emit error(
                    "Failed to load statistics: " + reply->errorString()
                );
            }
            reply->deleteLater();
        },
        "GET"
    );
}

void NetworkManager::createTask(const QJsonObject &task) {
    QJsonObject requestData;
    requestData["username"] = currentUserName;
    requestData["title"] = task["title"];
    requestData["difficulty"] = task["difficulty"];
    requestData["description"] = task["description"];
    requestData["tests"] = task["tests"];

    sendRequest(
        "/tasks", requestData,
        [this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QJsonObject jsonObj =
                    QJsonDocument::fromJson(reply->readAll()).object();
                int taskId = jsonObj["task_id"].toInt();
                emit taskCreated(taskId);
            } else {
                emit error("Failed to edit task: " + reply->errorString());
            }
            reply->deleteLater();
        },
        "POST"
    );
}

void NetworkManager::editTask(int taskId, const QJsonObject &taskData) {
    QString url = "/tasks/" + QString::number(taskId);

    QJsonObject requestData;

    if (taskData.contains("title")) {
        requestData["title"] = taskData["title"];
    }
    if (taskData.contains("description")) {
        requestData["description"] = taskData["description"];
    }
    if (taskData.contains("difficulty")) {
        requestData["difficulty"] = taskData["difficulty"];
    }
    if (taskData.contains("tests")) {
        requestData["tests"] = taskData["tests"];
    }

    requestData["username"] = currentUserName;

    sendRequest(
        url, requestData,
        [this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                emit taskEdited();
            } else {
                emit error("Failed to edit task: " + reply->errorString());
            }
            reply->deleteLater();
        },
        "PUT"
    );
}

void NetworkManager::deleteTask(int taskId) {
    QString url = "/tasks/" + QString::number(taskId);
    QJsonObject requestData;

    requestData["username"] = currentUserName;
    sendRequest(
        url, requestData,
        [this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                emit taskDeleted();
            } else {
                emit error("Failed to delete task: " + reply->errorString());
            }
            reply->deleteLater();
        },
        "DELETE"
    );
}

void NetworkManager::sendRequest(
    const QString &urlEnd,
    const QJsonObject &data,
    std::function<void(QNetworkReply *)> callback,
    const QString &method
) {
    QUrl url(baseUrl + urlEnd);

    qDebug() << "----- ОТПРАВКА ЗАПРОСА -----";
    qDebug() << "Тип:" << method;
    qDebug() << "URL:" << url.toString();
    qDebug() << "Данные:" << QJsonDocument(data).toJson();

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nullptr;
    if (method == "GET") {
        reply = manager->get(request);
    } else if (method == "POST") {
        QJsonDocument document(data);
        reply = manager->post(request, document.toJson());
    } else if (method == "PUT") {
        QJsonDocument document(data);
        reply = manager->put(request, document.toJson());
    } else if (method == "DELETE") {
        QByteArray body = QJsonDocument(data).toJson();
        reply = manager->sendCustomRequest(request, "DELETE", body);
    }

    connect(reply, &QNetworkReply::finished, [reply, callback]() {
        callback(reply);
    });
}

void NetworkManager::setCurrentUser(
    const QString &userName,
    const QString &role,
    int userId
) {
    currentUserName = userName;
    currentUserRole = role;
    currentUserId = userId;
}

QString NetworkManager::getRole() {
    return currentUserRole;
}

QString NetworkManager::getUserName() {
    return currentUserName;
}

int NetworkManager::getUserId() {
    return currentUserId;
}

void NetworkManager::logout()
{
    currentUserId = -1;
    currentUserName.clear();
    currentUserRole.clear();
}
