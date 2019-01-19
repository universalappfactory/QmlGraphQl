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

#include "graphqlerror.h"
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

QString GraphQlError::message() const
{
    return m_message;
}

void GraphQlError::setMessage(const QString &message)
{
    m_message = message;
}

QVariantMap GraphQlError::toVariantmap()
{
    QString json = "{ \"message\": \"" + m_message + +"\"}";

    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    return doc.object().toVariantMap();
}

GraphQlError::GraphQlError(const QString &message) :
    m_message(message)
{
}
