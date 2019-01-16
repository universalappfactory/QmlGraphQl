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
