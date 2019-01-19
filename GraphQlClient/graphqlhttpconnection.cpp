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
#include <QDebug>
#include <QJsonDocument>

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
        QJsonDocument response = QJsonDocument::fromJson(data);

        if (!response.isNull())         {
            emit dataReceived(response.object().toVariantMap());
        } else {
            qDebug() << "error parsing response" << data;
        }
    }

    reply->deleteLater();
}
