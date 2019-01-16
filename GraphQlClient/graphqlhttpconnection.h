#ifndef GRAPHQLHTTPCONNECTION_H
#define GRAPHQLHTTPCONNECTION_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "queryrequestdto.h"

class GraphQlHttpConnection : public QObject
{
    Q_OBJECT
public:
    explicit GraphQlHttpConnection(QObject *parent = nullptr);
    ~GraphQlHttpConnection();

    QString url() const;
    void setUrl(const QString &url);
    void sendMessage(const QueryRequestDto &message);

signals:
    void dataReceived(QVariantMap data);

public slots:

private Q_SLOTS:
    void onFinished(QNetworkReply *reply);

private:
    QString m_url;
    QNetworkAccessManager *m_networkAccessManager;

};

#endif // GRAPHQLHTTPCONNECTION_H
