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
