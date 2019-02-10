/*
    This file is part of QmlGraphQl.

    QmlGraphQl is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QmlGraphQl is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QmlGraphQl.  If not, see <https://www.gnu.org/licenses/>.
*/
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

    Q_INVOKABLE QString query(const QString &query);
    Q_INVOKABLE void mutate(const QString &mutation);
    Q_INVOKABLE void open();
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(WebSocketConnectionState websocketConnectionState READ websocketConnectionState NOTIFY websocketConnectionStateChanged)
    Q_PROPERTY(bool isConnected READ isConnected)

    QString url() const;

    WebSocketConnectionState websocketConnectionState() const;
    bool isConnected() const;

public slots:
    void onConnectionError(GraphQlError err);
    void setUrl(const QString &url);
    void onStateChanged(GraphQlWebsocketConnection::ConnectionState state);


Q_SIGNALS:
    void closed();
    void urlChanged(QString url);
    void dataReceived(QVariantMap data);
    void websocketConnectionStateChanged(WebSocketConnectionState state);
    void error(QVariantMap error);

private:
     QString m_url;
     GraphQlWebsocketConnection *m_websocketConnection;
     GraphQlHttpConnection *m_httpConnection;
     void emitDataReceived(const OperationMessage &message);
};

#endif // GRAPHQLWEBSOCKET_H
