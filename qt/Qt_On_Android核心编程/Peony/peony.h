#ifndef PEONY_H
#define PEONY_H
#include <QObject>
#include <QTimer>
#include <QDateTime>

class Peony : public QObject
{
    Q_OBJECT
public:
    Peony();
    void setBloomDate(const QDateTime &date);

protected slots:
    void onBloomTime();

signals:
    void bloom();
    void wizen();

private:
    QTimer * m_timer;
};

#endif // PEONY_H
