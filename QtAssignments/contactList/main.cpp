#include <QApplication>
#include "qmlapplicationviewer.h"
#include "uicontroller.h"
#include <QtDeclarative>
#include "contactsmodel.h"
#include<QDebug>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    QmlApplicationViewer viewer;
    qmlRegisterType<UiController>("IUiManager",1,1,"UiManager");
    qmlRegisterType<QAppManager>("IQApp",1,1,"QApp");
    //UiController uiController;
    //QTimer::singleShot(5000, &uiController, SLOT(handleDateTime()));
    //QTimer *timer = new QTimer(&uiController);
    //QObject::connect(timer, SIGNAL(timeout()), &uiController, SLOT(handleDateTime()));
    //timer->start(5000);
    //QDeclarativeView view;


    viewer.rootContext()->setContextProperty("testModel", ContactsModel::getInstance());

    //QAppManager qAppManager;
    //QStringList myContactsModel;
   // myContactsModel=qAppManager.getContacts();

    //qDebug()<<myContactsModel;

    viewer.addImportPath(QLatin1String("modules"));
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/contactList/main.qml"));

    viewer.showExpanded();


    //view.setSource(QUrl::fromLocalFile("qml/contactList/main.qml"));
         //view.show();

    return app->exec();
}
