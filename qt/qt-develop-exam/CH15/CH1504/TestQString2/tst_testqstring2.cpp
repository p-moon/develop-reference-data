#include <QString>
#include <QtTest>

class TestQString2 : public QObject
{
    Q_OBJECT
    
public:
    TestQString2();
    
private Q_SLOTS:
    void testBenchmark();
};

TestQString2::TestQString2()
{
}

void TestQString2::testBenchmark()
{
    QString str("heLLO");
    // 要用来测试性能的代码
    QBENCHMARK
    {
        str.toLower();
    }
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(TestQString2)

#include "tst_testqstring2.moc"
