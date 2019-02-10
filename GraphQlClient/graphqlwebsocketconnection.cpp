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

#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include "graphqlwebsocketconnection.h"
#include "graphqlerror.h"

void GraphQlWebsocketConnection::setUrl(const QString &url)
{
    m_url = url;
}

GraphQlWebsocketConnection::ConnectionState GraphQlWebsocketConnection::connectionState() const
{
    return m_connectionstate;
}

void GraphQlWebsocketConnection::close()
{
    m_webSocket.close();
}

GraphQlWebsocketConnection::GraphQlWebsocketConnection(QObject *parent) : QObject(parent),
    m_url(QString()),
    m_connectionstate(GraphQlWebsocketConnection::ConnectionState::Disconnected)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &GraphQlWebsocketConnection::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &GraphQlWebsocketConnection::closed);
    connect(&m_webSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &GraphQlWebsocketConnection::onTextMessageReceived);
}


void GraphQlWebsocketConnection::sendMessage(const OperationMessage &message)
{
    QString json = message.toJson();
    qDebug() << "sending: " << json;
    m_webSocket.sendTextMessage(json);
}


void GraphQlWebsocketConnection::open()
{
    QNetworkRequest request = QNetworkRequest(m_url);
    request.setRawHeader("Sec-WebSocket-Protocol","graphql-ws");
    m_webSocket.open(request);
}

void GraphQlWebsocketConnection::onConnected()
{
    qDebug() << "WebSocket connected";

    m_connectionstate = Initializing;
    sendMessage(OperationMessage::ConnectionInitMessage());
    emit connected();
}

void GraphQlWebsocketConnection::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message;

    OperationMessage operationMessage = OperationMessage::fromJson(message.toUtf8());

    switch (m_connectionstate) {
        case GraphQlWebsocketConnection::ConnectionState::Acknowledged:
            if (operationMessage.type() == OperationMessage::GQL_DATA) {
                emitDataReceived(operationMessage);
            }
            break;
        case GraphQlWebsocketConnection::ConnectionState::Initializing:
            if (operationMessage.type() == OperationMessage::GQL_CONNECTION_ACK) {
                m_connectionstate = ConnectionState::Acknowledged;
                emit stateChanged(m_connectionstate);
            }
            break;
        case GraphQlWebsocketConnection::ConnectionState::Disconnected:
            break;
        case GraphQlWebsocketConnection::ConnectionState::Error:
            break;
    }
}

void GraphQlWebsocketConnection::emitDataReceived(const OperationMessage &message)
{
    qDebug() << "emitDataReceived:" << message.toJson();

    //QJsonDocument doc = QJsonDocument::fromVariant(message.payload());
    emit dataReceived(message.toVariantMap());
}

void GraphQlWebsocketConnection::onError(QAbstractSocket::SocketError socktError)
{
    qDebug() << "error" << socktError;
    qDebug() << m_webSocket.errorString();
    m_connectionstate = Error;

    if (m_webSocket.state() == QAbstractSocket::SocketState::ConnectedState)
        m_webSocket.close();

    GraphQlError err = GraphQlError(m_webSocket.errorString());
    emit error(err);
}

QString GraphQlWebsocketConnection::url() const
{
    return m_url;
}
