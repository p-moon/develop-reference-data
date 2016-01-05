#include <QString>
#include <QtTest>
#include "area.h"

class TestArea : public QObject
{
    Q_OBJECT
    
public:
    TestArea();
    
private Q_SLOTS:
    void toArea();      //测试函数名toArea()
    void toArea_data(); //初始化数据的函数名toArea_data()和测试函数名toArea()一样，但增加了后缀_data
};

TestArea::TestArea()
{
}

void TestArea::toArea()
{
    //取测试数据
    QFETCH(Area,area);
    QFETCH(double,r);
    QVERIFY(qAbs(area.CountArea()-r)<0.0000001);
    QVERIFY2(true, "Failure");
}

void TestArea::toArea_data()
{
    //定义测试数据列
    QTest::addColumn<Area>("area");
    QTest::addColumn<double>("r");
    //建立测试数据
    QTest::newRow("1")<<Area(1)<<3.14;
    QTest::newRow("2")<<Area(2)<<12.56;
    QTest::newRow("3")<<Area(3)<<28.26;
}

QTEST_APPLESS_MAIN(TestArea)

#include "tst_testarea.moc"
