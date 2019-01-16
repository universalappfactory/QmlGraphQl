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


