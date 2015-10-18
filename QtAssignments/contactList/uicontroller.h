#ifndef UICONTROLLER_H
#define UICONTROLLER_H
#include<QObject>
#include "qappmanager.h"
#include "qappmanager.h"
class UiController : public QObject
{
    Q_OBJECT
public:
    explicit UiController(QObject *parent = 0);

signals:
    void windowStatusChanged(int z1,int z2,int z3);
    void timeUpdated(QString dateString,QString timeString);
    void contactsFromUi(QStringList a);
public slots:
    void route();
    void handleBrowse();
    QString getTime();
    QString getDate();

private:
    QAppManager appManager;

};

#endif // UICONTROLLER_H
