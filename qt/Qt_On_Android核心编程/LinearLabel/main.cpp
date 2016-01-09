#include "linearlabel.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont f = a.font();
    f.setPixelSize(60);
    QWidget w;
    QVBoxLayout *layout = new QVBoxLayout(&w);
    LinearLabel *ll = new LinearLabel("LinearLabel");
    ll->setFont(f);
    ll->setPenWidth(2);
    ll->setColorAt(0, QColor(255, 255, 255));
    ll->setColorAt(0.5, QColor(100, 0, 0));
    ll->setColorAt(1, QColor(255, 0, 0));
    layout->addWidget(ll);
    layout->addStretch(1);

    w.show();

    return a.exec();
}
