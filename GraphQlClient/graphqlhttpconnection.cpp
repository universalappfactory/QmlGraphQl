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
