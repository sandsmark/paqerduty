#include "Notification.h"
#include "Poller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("martin");

    Notification w;
    Poller poller;

    QObject::connect(&poller, &Poller::newEvent, &w, &Notification::onNewEvent);
    QObject::connect(&poller, &Poller::noEvents, &w, &Notification::onNoEvents);

    return a.exec();
}
