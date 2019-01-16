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
#ifndef MYITEM_H
#define MYITEM_H

#include <QQuickItem>

class MyItem : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(MyItem)

    QString m_name;

public:
    MyItem(QQuickItem *parent = nullptr);
    ~MyItem();

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    QString name() const
    {
        return m_name;
    }
public slots:
    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(m_name);
    }
signals:
    void nameChanged(QString name);
};

#endif // MYITEM_H
