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
#ifndef QUERYREQUESTDTO_H
#define QUERYREQUESTDTO_H

#include <QString>
#include <QJsonObject>

class QueryRequestDto
{

private:
    QString m_operationName;
    QString m_nameQuery;
    QString m_query;
    QJsonObject m_variables;

public:
    QueryRequestDto(const QString &operationName, const QString &nameQuery, const QString &query, const QJsonObject &variables);
    QueryRequestDto(const QString &query);

    QString operationName() const;
    QString nameQuery() const;
    QString query() const;
    QJsonObject variables() const;
    QJsonObject toJsonObject() const;
    QByteArray toByteArray() const;
};

#endif // QUERYREQUESTDTO_H
