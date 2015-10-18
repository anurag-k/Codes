#include "adaptor.h"
#include "contactdbservice.h"
#include "contactinfo.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //QDBusConnectionInterface* interface = QDBusConnection::sessionBus().interface();


    ContactDBService* demo = new ContactDBService;
    new Adaptor(demo);

    QDBusConnection connection = QDBusConnection::sessionBus();
    registerMetaType();
    bool ret = connection.registerService("com.contact.info");
    ret = connection.registerObject("/DemoObject", demo);
    return a.exec();
}
