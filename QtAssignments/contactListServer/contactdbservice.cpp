#include "contactdbservice.h"
#include "contactinfo.h"
#include<QFile>
//#include<QFileDialog>
#include<QXmlStreamReader>
#include<QDebug>
ContactDBService::ContactDBService(QObject *parent) :
    QObject(parent)
{
    /*member[0].name="Arun Kumar";
    member[0].mobile="\t9123455420";
    member[0].email="\tarun@gmail.com";

    member2.name="Bhargav Mehta";
    member2.mobile="\t9188888620";
    member2.email="\tbharagava@yahoo.co.in";

    list1.append(member1);
    list1.append(member2);*/
    int i=0;
    QString path = "/home/kpit/Documents/contactbook.xml";/*QFileDialog::getOpenFileName("contactbook");*/
    QFile file(path);
    //if(!file.open(QFile::ReadOnly|QIODevice::Text))
      //  return;
    file.open(QFile::ReadOnly|QIODevice::Text);
    QXmlStreamReader reader(&file);

    while(!reader.atEnd())
    {
        if(reader.name() != "user")
            reader.readNextStartElement();
        if(reader.name() == "user")
        {
            //qDebug() << reader.name();
            //QString name,mobile,email;
            while(reader.readNextStartElement())
            {
                QStringRef name1 = reader.name();
                if(name1 == "user")
                    break;
                if(name1 == "name")
                {
                    memberList[i].name = reader.readElementText();
                    qDebug()<<memberList[i].name;
                }
                else if(name1 == "mob")
                {
                    memberList[i].mobile = reader.readElementText();
                    qDebug()<<memberList[i].mobile;
                }
                else if(name1 == "email")
                {
                    memberList[i].email = reader.readElementText();
                    qDebug()<<memberList[i].email;
                }
            }
            list1.append(memberList[i]);
            i++;

            //if(!name.isEmpty() && !mobile.isEmpty() && !email.isEmpty())
            //    model.addPerson(PersonalData(firstname,lastname,email));
        }
    }

}
ContactDBService::~ContactDBService()
{

}

contactList ContactDBService::GetContactDetails()
{
    // handle method call com.contact.info.GetContactDetails
//    member contactList;
//    QMetaObject::invokeMethod(parent(), "GetContactDetails", Q_RETURN_ARG(member, contactList));
//    return contactList;
    return list1;
}
