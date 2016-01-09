#ifndef PEONYLOVER_H
#define PEONYLOVER_H
#include <QObject>
#include <QDebug>

class PeonyLover : public QObject
{
    Q_OBJECT
public:
    PeonyLover(QString name) : m_strName(name)
    {

    }

public slots:
    void onPeonyBloom()
    {
        qDebug() << "peony bloom, let\'s go luoyang";
    }

private:
    QString m_strName;
};

#endif // PEONYLOVER_H
