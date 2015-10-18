
#include "contactsmodel.h"
#include<QDebug>
ContactsModel* ContactsModel::instanceOfContactsModel=NULL;
//TestModel::TestModel(QObject *parent) :
//    QObject(parent)
//{
//}
//ContactsModel::ContactsModel(QObject *parent)
//{

//}
//ContactsModel::ContactsModel(QObject *parent):QAbstractListModel(parent)
ContactsModel::ContactsModel()
{
    qDebug()<<"\n constructor";
}


int ContactsModel::rowCount(const QModelIndex &parent) const
 {
     return stringList.count();
 }
QVariant ContactsModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (index.row() >= stringList.size() || index.row()<0)
         return QVariant();

     //if (role == Qt::DisplayRole || role == Qt::EditRole)
     if(role==ContactFieldRole)
         return stringList.at(index.row());
     else
         return QVariant();
 }
Qt::ItemFlags ContactsModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return Qt::ItemIsEnabled;

     return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
 }
bool ContactsModel::setData(const QModelIndex &index,
                              const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        stringList.replace(index.row(), value.toString());
        emit dataChanged(index,index);
        return true;
    }
    return false;
}
bool ContactsModel::insertRows(int position, int rows, const QModelIndex &parent)
 {
     beginInsertRows(QModelIndex(), position, position+rows-1);

     for (int row = 0; row < rows; ++row) {
         stringList.insert(position, "");
     }

     endInsertRows();
     return true;
 }
void ContactsModel::addContact(const QString& contact)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    stringList<<contact;
    endInsertRows();

}
QHash<int, QByteArray> ContactsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ContactFieldRole] = "somecontact";

    return roles;
}
ContactsModel*ContactsModel::getInstance()
{
    qDebug()<<"getInstance function called";
    if(instanceOfContactsModel==NULL)
    {
        instanceOfContactsModel=new ContactsModel;
        qDebug()<<"new instance created";
    }
     qDebug()<<"instance returned";
    return instanceOfContactsModel;
}

/*void ContactsModel::updateList()
{
    this->addContact("something");
}*/
