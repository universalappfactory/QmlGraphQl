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

#include <QtCore/QDebug>
#include <QAbstractSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QUuid>
#include <QString>
#include "queryrequestdto.h"
#include "graphqlconnection.h"
#include "graphqlerror.h"


GraphQlConnection::GraphQlConnection() :
    m_url(QString()),
    m_websocketConnection(new GraphQlWebsocketConnection(this)),
    m_httpConnection(new GraphQlHttpConnection(this)),
    m_authorizationHeader(QString())
{
    connect(m_websocketConnection, &GraphQlWebsocketConnection::dataReceived, this, &GraphQlConnection::dataReceived);
    connect(m_websocketConnection, &GraphQlWebsocketConnection::stateChanged, this, &GraphQlConnection::onStateChanged);
    connect(m_websocketConnection, &GraphQlWebsocketConnection::error, this, &GraphQlConnection::onConnectionError);

    connect(m_httpConnection, &GraphQlHttpConnection::dataReceived, this, &GraphQlConnection::dataReceived);
    connect(m_httpConnection, &GraphQlHttpConnection::error, this, &GraphQlConnection::onConnectionError);
}

GraphQlConnection::~GraphQlConnection()
{
    delete m_websocketConnection;
    delete m_httpConnection;
}

QString GraphQlConnection::query(const QString &query)
{
    qDebug() << "query: " << query;

    if (wsUrl().isEmpty() || websocketConnectionState() !=  WebSocketConnectionState::Acknowledged) {
        qDebug() << "websocket not available, doing http request";
        return m_httpConnection->sendMessage(QueryRequestDto(query));
    }

    OperationMessage operationMessage = OperationMessage::ConnectionStartMessage(QueryRequestDto(query).toJsonObject());
    m_websocketConnection->sendMessage(operationMessage);
    return operationMessage.id();
}

QString GraphQlConnection::subscribe(const QString &subscription)
{
    if (websocketConnectionState() !=  WebSocketConnectionState::Acknowledged) {
        emitWebsocketConnectionRequiredError();
        return emptyUid();
    }

    qDebug() << "subscribe: " << subscription;
    OperationMessage operationMessage = OperationMessage::ConnectionStartMessage(QueryRequestDto(subscription).toJsonObject());
    m_websocketConnection->sendMessage(operationMessage);
    return operationMessage.id();
}

void GraphQlConnection::unsubscribe(const QString &subscriptionId)
{
    qDebug() << "unsubscribe: " << subscriptionId;
    OperationMessage operationMessage = OperationMessage::ConnectionStopMessage(subscriptionId);
    m_websocketConnection->sendMessage(operationMessage);
}

QString GraphQlConnection::mutate(const QString &mutation)
{
    qDebug() << "mutate: " << mutation;

    if (wsUrl().isEmpty() || websocketConnectionState() !=  WebSocketConnectionState::Acknowledged) {
        qDebug() << "websocket not available, doing http request";
        return m_httpConnection->sendMessage(QueryRequestDto(mutation));
    }

    OperationMessage operationMessage = OperationMessage::ConnectionStartMessage(QueryRequestDto(mutation).toJsonObject());
    m_websocketConnection->sendMessage(operationMessage);
    return operationMessage.id();
}

void GraphQlConnection::open()
{
    m_websocketConnection->open();
}

QString GraphQlConnection::url() const
{
    return m_url;
}

QString GraphQlConnection::wsUrl() const
{
    return m_wsUrl;
}

QString GraphQlConnection::authorizationHeader() const
{
    return m_authorizationHeader;
}

GraphQlConnection::WebSocketConnectionState GraphQlConnection::websocketConnectionState() const
{
    return static_cast<WebSocketConnectionState>(m_websocketConnection->connectionState());
}

bool GraphQlConnection::isConnected() const
{
    return websocketConnectionState() == WebSocketConnectionState::Acknowledged;
}

void GraphQlConnection::onConnectionError(GraphQlError err)
{
    emit error(err.toVariantmap());
}

void GraphQlConnection::setUrl(const QString &url)
{
    if (m_url == url)
        return;

    m_url = url;
    m_httpConnection->setUrl(url);
    emit urlChanged(m_url);
}

void GraphQlConnection::setWsUrl(QString wsUrl)
{
    if (m_wsUrl == wsUrl)
        return;

    m_wsUrl = wsUrl;
    m_websocketConnection->setUrl(m_wsUrl);
    emit wsUrlChanged(m_wsUrl);
}

void GraphQlConnection::setAuthorizationHeader(QString authorizationHeader)
{
    if (authorizationHeader == m_authorizationHeader)
        return;

    m_authorizationHeader = authorizationHeader;
    m_httpConnection->setAuthorizationHeader(m_authorizationHeader);
    emit authorizationHeaderChanged(m_authorizationHeader);
}

void GraphQlConnection::onStateChanged(GraphQlWebsocketConnection::ConnectionState state)
{
    emit websocketConnectionStateChanged(static_cast<WebSocketConnectionState>(state));
}

QString GraphQlConnection::emptyUid()
{
    return QUuid().toString();
}

void GraphQlConnection::emitWebsocketConnectionRequiredError()
{
    emit error(GraphQlError("a websocket open websocket-connection is required.").toVariantmap());
}


