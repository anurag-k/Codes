#include "uicontroller.h"
#include<QDate>
#include<QTime>
#include<QDebug>
UiController::UiController(QObject *parent) :
    QObject(parent)
{

}

void UiController::route()
{
    //QStringList a;
    emit windowStatusChanged(0,1,0);
    appManager.getContacts();
    //a=appManager.myContactsModel;
    //emit contactsFromUi(a);
}
void UiController::handleBrowse()
{
    emit windowStatusChanged(0,0,1);
}
QString UiController::getTime()
{

    QTime time = QTime::currentTime();
    QString timeString = time.toString();
    timeString.remove(5,7);
    //emit timeUpdated(dateString,timeString);
    return timeString;
}
QString UiController::getDate()
{
    QDate date = QDate::currentDate();
    QString dateString = date.toString();
    return dateString;
}

