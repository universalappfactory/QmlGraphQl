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
