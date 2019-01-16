#ifndef GRAPHQLWEBSOCKETCONNECTION_H
#define GRAPHQLWEBSOCKETCONNECTION_H

#include <QObject>
#include <QWebSocket>
#include <QString>
#include <QVariantMap>
#include "operationmessage.h"

class GraphQlWebsocketConnection : public QObject
{
    Q_OBJECT
public:

    enum ConnectionState {
        Disconnected = 0,
        Initializing = 1,
        Acknowledged = 2,
        Error = 3
    };

    Q_ENUM(ConnectionState)

    explicit GraphQlWebsocketConnection(QObject *parent = nullptr);
    void sendMessage(const OperationMessage &message);
    void open();
    QString url() const;
    void setUrl(const QString &url);
    ConnectionState connectionState() const;
    void close();

signals:
    void connected();
    void dataReceived(QVariantMap data);
    void error(QAbstractSocket::SocketError error);
    void closed();
    void stateChanged(ConnectionState state);

public slots:

private Q_SLOTS:
    void onError(QAbstractSocket::SocketError socktError);
    void onConnected();
    void onTextMessageReceived(QString message);

private:
    QWebSocket m_webSocket;
    QString m_url;
    ConnectionState m_connectionstate;

    void emitDataReceived(const OperationMessage &message);
};

#endif // GRAPHQLWEBSOCKETCONNECTION_H
