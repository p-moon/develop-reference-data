#include <QCoreApplication>
#include "peony.h"
#include "peonyLover.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Peony *peony = new Peony();
    peony->setBloomDate(QDateTime::currentDateTime().addDays(30));

    PeonyLover * jack = new PeonyLover("Jack");
    QObject::connect(peony, SIGNAL(bloom()), jack, SLOT(onPeonyBloom()));
    PeonyLover * zhangsan = new PeonyLover("zhang san");
    QObject::connect(peony, SIGNAL(bloom()), zhangsan, SLOT(onPeonyBloom()));

    return a.exec();
}
