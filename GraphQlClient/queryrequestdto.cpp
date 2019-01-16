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
#include "queryrequestdto.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

QueryRequestDto::QueryRequestDto(const QString &operationName, const QString &nameQuery, const QString &query, const QJsonObject &variables) :
        m_operationName(operationName),
        m_nameQuery(nameQuery),
        m_query(query),
        m_variables(variables)
{

}

QueryRequestDto::QueryRequestDto(const QString &query) : QueryRequestDto(QString(),QString(),query,QJsonObject())
{

}

QString QueryRequestDto::operationName() const
{
    return m_operationName;
}

QString QueryRequestDto::nameQuery() const
{
    return m_nameQuery;
}

QString QueryRequestDto::query() const
{
    return m_query;
}

QJsonObject QueryRequestDto::variables() const
{
    return m_variables;
}

QJsonObject QueryRequestDto::toJsonObject() const
{
    QJsonValue variables = m_variables.isEmpty() ? QJsonValue() : QJsonValue(m_variables);

    QJsonObject object
    {
        {"operationName", m_operationName},
        {"namedQuery", m_nameQuery},
        {"query", m_query},
        {"variables", variables},
    };

    return  object;
}

QByteArray QueryRequestDto::toByteArray() const
{
    QJsonDocument doc(toJsonObject());
    return doc.toJson(QJsonDocument::Compact);
}


