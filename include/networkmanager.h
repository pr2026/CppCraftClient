#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class NetworkManager : public QObject {
    Q_OBJECT
public:
    static NetworkManager *instance();
    void login(const QString &username, const QString &password);
    void sendRequest(
        const QString &urlEnd,
        const QJsonObject &data,
        std::function<void(QNetworkReply *)> callback
    );

private:
    explicit NetworkManager(QObject *parent = nullptr);
    static NetworkManager *m_instance;
    QNetworkAccessManager *manager;
    QString baseUrl = "http://192.168.107.9:8080";

signals:
    void loginSuccess();
    void loginError(const QString &message);
};

#endif  // NETWORKMANAGER_H
