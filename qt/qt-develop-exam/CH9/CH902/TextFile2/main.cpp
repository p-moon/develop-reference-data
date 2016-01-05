#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile data("data.txt");
    if(data.open(QFile::WriteOnly|QFile::Truncate))
    {
        QTextStream out(&data);
        out<<QObject::tr("score:")<<qSetFieldWidth(10)<<left<<90<<endl;
    }
    
    return a.exec();
}
