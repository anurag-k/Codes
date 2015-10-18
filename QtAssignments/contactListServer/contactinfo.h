#ifndef CONTACTINFO_H
#define CONTACTINFO_H

#include <QObject>
#include<QMetaType>
#include <QtDBus/QDBusMetaType>
#include<QtDBus/QDBusArgument>
#include<QString>
#include<QList>
struct member
{
    QString name;
    QString mobile;
    QString email;
};
Q_DECLARE_METATYPE(member)
typedef QList<member> contactList;
Q_DECLARE_METATYPE(contactList)
QDBusArgument &operator<<(QDBusArgument &argument, const member& p_member);

// Retrieve the radio_t data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, member& p_member);
inline void registerMetaType()
{
    qDBusRegisterMetaType < member >();
    qDBusRegisterMetaType < contactList >();
}

#endif // CONTACTINFO_H
