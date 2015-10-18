#include "qappmanager.h"
#include<QDebug>


#include "qmlapplicationviewer.h"
#include "uicontroller.h"
#include <QtDeclarative>
#include "contactinfo.h"
QAppManager::QAppManager(QObject *parent) :
    QObject(parent)
{
    registerMetaType();

    m_Proxy = new Proxy("com.contact.info","/DemoObject",QDBusConnection::sessionBus(),NULL);
}
void QAppManager::getContacts()
{

    //qDebug()<<"App Manager Class";

    //myContactsModel=contactsModel.toContactsList();
    //Q_EMIT contactListObtained(myContactsModel);
    contactList listOfContacts;
    listOfContacts=m_Proxy->GetContactDetails();


    QString contactField;
    QStringList contactFieldList;


    for(int i=0;i<listOfContacts.count();i++)
    {
        contactField=listOfContacts[i].name;
        //contactField.append("   ");
        contactField.append(listOfContacts[i].mobile);
        //contactField.append("   ");
        contactField.append(listOfContacts[i].email);
        contactFieldList<<contactField;

        (ContactsModel::getInstance())->addContact(contactField);

    }


    /*contactField=listOfContacts[0].name;
    contactField.append("   ");
    contactField.append(listOfContacts[0].mobile);
    contactField.append("   ");
    contactField.append(listOfContacts[0].email);*/
    myContactsModel=contactFieldList;


    //qDebug()<<contactField;
    qDebug()<<contactFieldList;
    //qDebug()<<listOfContacts[0];
    //emit contactListObtained(contactFieldList);
    //return myContactsModel;


}
/*QStringList QAppManager::getContacts(int a)
{
    return myContactsModel;
}*/
