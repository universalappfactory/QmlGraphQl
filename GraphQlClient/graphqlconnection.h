#ifndef GRAPHQLWEBSOCKET_H
#define GRAPHQLWEBSOCKET_H

#include <QQuickItem>
#include <QtWebSockets/QWebSocket>
#include <QJsonObject>
#include <QVariantMap>
#include "operationmessage.h"
#include "graphqlwebsocketconnection.h"
#include "graphqlhttpconnection.h"

class GraphQlConnection : public QQuickItem
{
    Q_OBJECT
public:
    GraphQlConnection();
    ~GraphQlConnection();

    enum WebSocketConnectionState {
           Disconnected = GraphQlWebsocketConnection::Disconnected,
           Initializing = GraphQlWebsocketConnection::Initializing,
           Acknowledged = GraphQlWebsocketConnection::Acknowledged,
           Error = GraphQlWebsocketConnection::Error
       };
    Q_ENUM(WebSocketConnectionState)

    Q_INVOKABLE void query(const QString &query);
    Q_INVOKABLE void open();
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(WebSocketConnectionState websocketConnectionState READ websocketConnectionState NOTIFY websocketConnectionStateChanged)
    Q_PROPERTY(bool isConnected READ isConnected)

    QString url() const;

    WebSocketConnectionState websocketConnectionState() const;
    bool isConnected() const;

public slots:
    void onError(QAbstractSocket::SocketError error);
    void setUrl(const QString &url);
    void onStateChanged(GraphQlWebsocketConnection::ConnectionState state);


Q_SIGNALS:
    void closed();
    void urlChanged(QString url);
    void dataReceived(QVariantMap data);
    void websocketConnectionStateChanged(WebSocketConnectionState state);
    void error(QAbstractSocket::SocketError error);

private:
     QString m_url;
     GraphQlWebsocketConnection *m_websocketConnection;
     GraphQlHttpConnection *m_httpConnection;
     void emitDataReceived(const OperationMessage &message);
};

#endif // GRAPHQLWEBSOCKET_H
