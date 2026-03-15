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
            emit loginError("Server Error");
        }
        reply->deleteLater();
    });
}

void NetworkManager::sendRequest(
    const QString &urlEnd,
    const QJsonObject &data,
    std::function<void(QNetworkReply *)> callback
) {
    QUrl url(baseUrl + urlEnd);

    // qDebug() << "=== ОТПРАВКА ЗАПРОСА ===";
    // qDebug() << "URL:" << url.toString();
    // qDebug() << "Данные:" << QJsonDocument(data).toJson();

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonDocument document(data);
    QNetworkReply *reply = manager->post(request, document.toJson());
    connect(reply, &QNetworkReply::finished, [reply, callback]() {
        callback(reply);
    });
}
