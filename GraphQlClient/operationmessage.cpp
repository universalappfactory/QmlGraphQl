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
#include "operationmessage.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QUuid>
#include <QDebug>

const QString OperationMessage::GQL_CONNECTION_KEEP_ALIVE = QString("ka");
const QString OperationMessage::GQL_CONNECTION_INIT = QString("connection_init");
const QString OperationMessage::GQL_CONNECTION_ACK = QString("connection_ack");
const QString OperationMessage::GQL_START = QString("start");
const QString OperationMessage::GQL_STOP = QString("stop");
const QString OperationMessage::GQL_DATA = QString("data");
const QString OperationMessage::GQL_ERROR = QString("error");
const QString OperationMessage::GQL_COMPLETE = QString("complete");

QString OperationMessage::id() const
{
    return m_id;
}

QString OperationMessage::type() const
{
    return m_type;
}

QJsonObject OperationMessage::payload() const
{
    return m_payload;
}

QByteArray OperationMessage::toByteArray() const
{
    return toJson().toUtf8();
}

QVariantMap OperationMessage::toVariantMap() const
{
    return toJsonDocument().object().toVariantMap();
}

OperationMessage OperationMessage::fromJson(const QByteArray &value)
{
    QJsonDocument doc = QJsonDocument::fromJson(value);

    QString id = doc["id"].isNull() ? QString("") : doc["id"].toString();
    QString type = doc["type"].isNull() ? QString("") : doc["type"].toString();
    QVariant payload = doc["payload"].isNull() ? QVariant() : doc["payload"].toVariant();

    QJsonDocument payloadDoc = QJsonDocument::fromVariant(payload);
    return OperationMessage(id, type, payloadDoc.object());
}

OperationMessage OperationMessage::ConnectionInitMessage()
{
    return OperationMessage(OperationMessage::GQL_CONNECTION_INIT);
}

OperationMessage OperationMessage::ConnectionStartMessage(const QJsonObject &payload)
{
    QUuid id = QUuid::createUuid();
    return OperationMessage(id.toString(),OperationMessage::GQL_START, payload);
}

OperationMessage OperationMessage::ConnectionStopMessage(const QString connectionId)
{
    return OperationMessage(connectionId,OperationMessage::GQL_STOP);
}

OperationMessage::OperationMessage(const QString &id,const QString &type,const QJsonObject &payload) :
    m_id(id),
    m_type(type),
    m_payload(payload)
{

}

OperationMessage::OperationMessage(const QString &id,const QString &type) : OperationMessage(id, type, QJsonObject())
{

}

OperationMessage::OperationMessage(const QString &type) : OperationMessage(QString(), type, QJsonObject())
{

}

QString OperationMessage::toJson() const
{
    auto doc = toJsonDocument();
    return doc.toJson(QJsonDocument::Compact);
}

QJsonDocument OperationMessage::toJsonDocument() const
{
    QJsonObject object
    {
        {"id", m_id},
        {"type", m_type},
        {"payload", QJsonValue(m_payload)},
    };

    return QJsonDocument(object);
}
