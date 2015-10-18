/*#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QObject>
#include<QStringList>
class ContactsModel : public QObject
{
    Q_OBJECT
public:
    explicit ContactsModel(QObject *parent = 0);
    QStringList toContactsList();
signals:
    
public slots:
private:
     QStringList contacts;
};

#endif // CONTACTSMODEL_H*/
#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QObject>
#include<QAbstractListModel>
#include<QStringList>

#include<QHash>
class ContactsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ListingRoles
    {
        ContactFieldRole=Qt::UserRole+1
    };

    //explicit TestModel(QObject *parent = 0);
    //ContactsModel(QObject *parent = 0);
    ContactsModel();
    //~ContactsModel();
    //ContactsModel();
   /* TestModel(const QStringList &strings, QObject *parent = 0)
             : QAbstractListModel(parent), stringList(strings) {}*/
    //TestModel(const QStringList strList):stringList(strList){}
    static ContactsModel* getInstance();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
        bool setData(const QModelIndex &index, const QVariant &value,
                     int role = Qt::EditRole);
        bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
        void addContact(const QString& contact);

        //QStringList getList();
signals:
protected:
        QHash<int, QByteArray> roleNames() const;

public slots:
        //void updateList();
private:
     QStringList stringList;
     static ContactsModel* instanceOfContactsModel;

};

#endif // TESTMODEL_H
