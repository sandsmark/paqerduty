#include "Notification.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Notification w;
    return a.exec();
}
