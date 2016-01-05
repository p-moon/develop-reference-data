#include <QCoreApplication>
#include <QtDebug>

class Area
{
public:
    Area(){}
    void setR(double r)
    {
        m_r=r;
    }

    double getR()
    {
        return m_r;
    }

    double getArea()
    {
        return getR()*getR()*3.142;
    }
private:
    double m_r;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Area area;
    area.setR(1);
    double d_area;
    d_area = area.getArea();

    qDebug()<<d_area;
    
    return a.exec();
}
