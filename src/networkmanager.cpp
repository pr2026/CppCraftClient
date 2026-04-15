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
            emit loginSuccess();
        } else {
            emit error("Server Error");
        }
        reply->deleteLater();
    });
}

void NetworkManager::registration(
    const QString &username,
    const QString &password
) {
    QJsonObject registrationData;
    registrationData["username"] = username;
    registrationData["password"] = password;

    sendRequest(
        "/registration", registrationData,
        [this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QJsonObject jsonObj =
                    QJsonDocument::fromJson(reply->readAll()).object();
                emit registrationSuccess();
            } else {
                emit error("Server Error");
            }
            reply->deleteLater();
        }
    );
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
    });
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
    } else {
        QJsonDocument document(data);
        reply = manager->post(request, document.toJson());
    }

    connect(reply, &QNetworkReply::finished, [reply, callback]() {
        callback(reply);
    });
}
