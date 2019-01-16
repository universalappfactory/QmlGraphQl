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
#ifndef OPERATIONMESSAGE_H
#define OPERATIONMESSAGE_H
#include <QString>
#include <QVariant>
#include <QJsonObject>

class OperationMessage
{
private:
    QString m_id;
    QString m_type;
    QJsonObject m_payload;

public:
    OperationMessage(const QString &type);
    OperationMessage(const QString &id,const QString &type);
    OperationMessage(const QString &id,const QString &type,const QJsonObject &payload);

    static const QString GQL_CONNECTION_KEEP_ALIVE;
    static const QString GQL_CONNECTION_INIT;
    static const QString GQL_CONNECTION_ACK;
    static const QString GQL_START;
    static const QString GQL_DATA;
    static const QString GQL_ERROR;
    static const QString GQL_COMPLETE;

    QString toJson() const;
    QString id() const;
    QString type() const;
    QJsonObject payload() const;
    QByteArray toByteArray() const;

    static OperationMessage fromJson(const QByteArray &value);
    static OperationMessage ConnectionInitMessage();
    static OperationMessage ConnectionStartMessage(const QJsonObject &payload);
};

#endif // OPERATIONMESSAGE_H
