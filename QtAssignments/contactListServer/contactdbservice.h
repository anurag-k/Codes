#ifndef CONTACTDBSERVICE_H
#define CONTACTDBSERVICE_H

#include <QObject>
#include "contactinfo.h"
class ContactDBService : public QObject
{
    Q_OBJECT
public:
    explicit ContactDBService(QObject *parent = 0);

    virtual ~ContactDBService();
signals:
    
public Q_SLOTS: // METHODS
    contactList GetContactDetails();
private:
    //member member1,member2;
    member memberList[10];
    contactList list1;
};

#endif // CONTACTDBSERVICE_H
