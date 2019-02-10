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

#include "graphqlconnection.h"
#include <QtCore/QDebug>
#include <QAbstractSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QUuid>
#include "queryrequestdto.h"

GraphQlConnection::GraphQlConnection() :
    m_url(QString()),
    m_websocketConnection(new GraphQlWebsocketConnection(this)),
    m_httpConnection(new GraphQlHttpConnection(this))
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
    if (websocketConnectionState() !=  WebSocketConnectionState::Acknowledged) {
        qDebug() << "connection is not acknowledged, doing http request";

        m_httpConnection->sendMessage(QueryRequestDto(query));
        return QUuid().toString(); //http connections dont't return an id at the moment
    }

    qDebug() << "query: " << query;
    OperationMessage operationMessage = OperationMessage::ConnectionStartMessage(QueryRequestDto(query).toJsonObject());
    m_websocketConnection->sendMessage(operationMessage);
    return operationMessage.id();
}

void GraphQlConnection::mutate(const QString &mutation)
{
    if (websocketConnectionState() !=  WebSocketConnectionState::Acknowledged) {
        qDebug() << "connection is not acknowledged, doing http request";

        m_httpConnection->sendMessage(QueryRequestDto(mutation));
        return;
    }

    qDebug() << "mutate: " << mutation;
    m_websocketConnection->sendMessage(OperationMessage::ConnectionStartMessage(QueryRequestDto(mutation).toJsonObject()));
}

void GraphQlConnection::open()
{
    m_websocketConnection->open();
}

QString GraphQlConnection::url() const
{
    return m_url;
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
    m_websocketConnection->setUrl(url);
    m_httpConnection->setUrl(url);
    emit urlChanged(m_url);
}

void GraphQlConnection::onStateChanged(GraphQlWebsocketConnection::ConnectionState state)
{
    emit websocketConnectionStateChanged(static_cast<WebSocketConnectionState>(state));
}


