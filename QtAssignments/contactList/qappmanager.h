#ifndef QAPPMANAGER_H
#define QAPPMANAGER_H

#include <QObject>
#include "contactsmodel.h"
#include "proxy.h"
class QAppManager : public QObject
{
    Q_OBJECT
public:
    explicit QAppManager(QObject *parent = 0);
    void getContacts();
    QStringList myContactsModel;
    //ContactsModel contactsModel;
signals:
    void contactListObtained(QStringList contactFieldList);
public slots:
private:
    //ContactsModel c_model;
    Proxy *m_Proxy;
    
};

#endif // QAPPMANAGER_H
