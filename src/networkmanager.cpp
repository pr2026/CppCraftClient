#include "../include/networkmanager.h"

NetworkManager *NetworkManager::m_instance = nullptr;

NetworkManager *NetworkManager::instance() {
    if (!m_instance) {
        m_instance = new NetworkManager();
    }
    return m_instance;
}

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent), manager(new QNetworkAccessManager(this)) {
}

void NetworkManager::login(const QString &username, const QString &password) {
    QJsonObject loginData;
    loginData["username"] = username;
    loginData["password"] = password;

    sendRequest("/login", loginData, [this](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonObject jsonObj =
                QJsonDocument::fromJson(reply->readAll()).object();
            QString role = jsonObj["role"].toString();
            emit loginSuccess(role);
        } else {
            emit error("Server Error");
        }
        reply->deleteLater();
    }, "GET");
}

void NetworkManager::registration(
    const QString &username,
    const QString &password, const QString &role
) {
    QJsonObject registrationData;
    registrationData["username"] = username;
    registrationData["password"] = password;
    registrationData["role"] = role;

    sendRequest(
        "/registration", registrationData,
        [this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QJsonObject jsonObj =
                    QJsonDocument::fromJson(reply->readAll()).object();
                QString role_ans = jsonObj["role"].toString();
                emit registrationSuccess(role_ans);
            } else {
                emit error("Server Error");
            }
            reply->deleteLater();
        }
    , "GET");
}

void NetworkManager::loadTasks() {
    sendRequest("/tasks", {}, [this](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonObject jsonObj =
                QJsonDocument::fromJson(reply->readAll()).object();
            emit tasksLoadSuccess(jsonObj);
        } else {
            emit error("Server Error");
        }
        reply->deleteLater();
    }, "GET");
}

void NetworkManager::loadTaskDetails(int taskId) {
    QString url = "/tasks/" + QString::number(taskId);
    sendRequest(url, {}, [this](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonObject jsonObj =
                QJsonDocument::fromJson(reply->readAll()).object();
            emit taskDetailsLoadSuccess(jsonObj);
        } else {
            emit error("Server Error");
        }
        reply->deleteLater();
    }, "GET");
}

void NetworkManager::sendSolution(int taskId, const QString &code) {
    QJsonObject submissionData;
    submissionData["task_id"] = taskId;
    submissionData["code"] = code;

    QString url = "/tasks/" + QString::number(taskId) + "/submit";
    sendRequest(url, submissionData, [this](QNetworkReply *reply) {
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
            emit solutionError("Sending error" + reply->errorString());
        }
        reply->deleteLater();
    }, "POST");
}

void NetworkManager::createTask(const QJsonObject &task) {
    sendRequest("/tasks", task, [this](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            emit taskCreated();
        } else {
            emit error("Failed to create task " + reply->errorString());
        }
        reply->deleteLater();
    }, "POST");
}

void NetworkManager::editTask(int taskId, const QJsonObject &taskData) {
    QString url = "/tasks/" + QString::number(taskId);

   // TODO: уточнить какой метод

    sendRequest(url, taskData, [this](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            emit taskEdited();
        } else {
            emit error("Failed to edit task " + reply->errorString());
        }
        reply->deleteLater();
    }, "PUT");
}

void NetworkManager::deleteTask(int taskId) {
    QString url = "/tasks/" + QString::number(taskId);
    sendRequest(url, QJsonObject(), [this](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            emit taskDeleted();
        } else {
            emit error("Failed to delete task " + reply->errorString());
        }
        reply->deleteLater();
    }, "DELETE");
}

void NetworkManager::sendRequest(
    const QString &urlEnd,
    const QJsonObject &data,
    std::function<void(QNetworkReply *)> callback,
    const QString &method
) {
    QUrl url(baseUrl + urlEnd);

    // qDebug() << "----- ОТПРАВКА ЗАПРОСА -----";
    // qDebug() << "URL:" << url.toString();
    // qDebug() << "Данные:" << QJsonDocument(data).toJson();

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
        reply = manager->deleteResource(request);
    }

    connect(reply, &QNetworkReply::finished, [reply, callback]() {
        callback(reply);
    });
}
