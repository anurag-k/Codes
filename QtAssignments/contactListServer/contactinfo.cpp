#include "contactinfo.h"

#include "adaptor.h"
QDBusArgument &operator<<(QDBusArgument &argument, const member& p_member)

{
    argument.beginStructure();
    argument << p_member.name
             << p_member.mobile
             << p_member.email;


    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, member& p_member)
{
    argument.beginStructure();
    argument >> p_member.name
             >> p_member.mobile
             >> p_member.email;

    argument.endStructure();
    return argument;
}


