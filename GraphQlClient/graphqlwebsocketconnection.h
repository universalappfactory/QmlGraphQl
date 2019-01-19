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
#ifndef GRAPHQLWEBSOCKETCONNECTION_H
#define GRAPHQLWEBSOCKETCONNECTION_H

#include <QObject>
#include <QWebSocket>
#include <QString>
#include <QVariantMap>
#include "operationmessage.h"
#include "graphqlerror.h"

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
    void error(GraphQlError error);
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
