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
    QString sendMessage(const QueryRequestDto &message);

signals:
    void dataReceived(QVariantMap data);
    void error(QString error);

public slots:

private Q_SLOTS:
    void onFinished(QNetworkReply *reply);

private:
    QString m_url;
    QNetworkAccessManager *m_networkAccessManager;

};

#endif // GRAPHQLHTTPCONNECTION_H
