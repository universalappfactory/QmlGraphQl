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

#include "graphqlhttpconnection.h"
#include "operationmessage.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonDocument>
#include <QJsonValue>
#include <QUuid>

GraphQlHttpConnection::GraphQlHttpConnection(QObject *parent) : QObject(parent),
  m_networkAccessManager(new QNetworkAccessManager(this))
{
    connect(m_networkAccessManager, &QNetworkAccessManager::finished,
            this, &GraphQlHttpConnection::onFinished);
}

GraphQlHttpConnection::~GraphQlHttpConnection()
{
    delete m_networkAccessManager;
}

QString GraphQlHttpConnection::url() const
{
    return m_url;
}

void GraphQlHttpConnection::setUrl(const QString &url)
{
    m_url = url;
}

void GraphQlHttpConnection::sendMessage(const QueryRequestDto &message)
{
    QNetworkRequest request(m_url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,"application/json");
    //request.setRawHeader(QByteArray("X-Request-ID"),QByteArray("requestId"));
    m_networkAccessManager->post(request, message.toByteArray());
}

void GraphQlHttpConnection::onFinished(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
        emit error(reply->errorString());
    } else {
        QByteArray data = reply->readAll();

        //create a json document from response
        QJsonDocument response = QJsonDocument::fromJson(data);

        //create a new JsonDocument which is compatible to a OperationMessage
        //this is required in order to make http and websocket calls compatible
        QJsonObject obj = QJsonObject();
        obj.insert("id",QUuid().toString()); //http queries don't have id's at the moment
        obj.insert("payload",QJsonValue(response.object()));
        obj.insert("type","data");

        auto operationMessage = OperationMessage::fromJson(QJsonDocument(obj).toJson());
        emit dataReceived(operationMessage.toVariantMap());
    }

    reply->deleteLater();
}
