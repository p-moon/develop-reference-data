#ifndef AREA_H
#define AREA_H
#include <QObject>

class Area:public QObject
{
    Q_OBJECT

public:
    Area(){}
    ~Area(){}
    Area(const Area &area)
    {
        m_r = area.m_r;
    }
    Area(int r)
    {
        m_r=r;
    }
    double CountArea()
    {
        return  3.14*m_r*m_r;
    }
private:
    double m_r;
};

#endif // AREA_H
