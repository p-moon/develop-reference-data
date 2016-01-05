#include <QString>
#include <QtTest>
#include "area.h"

class TestArea : public QObject
{
    Q_OBJECT
    
public:
    TestArea();
    
private Q_SLOTS:
    void toAreaTest();
};

TestArea::TestArea()
{
}

void TestArea::toAreaTest()
{
    Area area(1);
    QVERIFY(qAbs(area.CountArea()-3.14)<0.0000001);
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(TestArea)

#include "tst_testarea.moc"
