#include "peony.h"

Peony::Peony() : m_timer(0)
{
}

void Peony::setBloomDate(const QDateTime & date)
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onBloomTime()));
    m_timer->setSingleShot(true);
    m_timer->start(QDateTime::currentDateTime().msecsTo(date));
}

void Peony::onBloomTime()
{
    QDateTime currentDate = QDateTime::currentDateTime();
    m_timer->disconnect(SIGNAL(timeout()), this);
    connect(m_timer, SIGNAL(timeout()), this, SIGNAL(wizen()));
    m_timer->start(currentDate.msecsTo(currentDate.addDays(10)));
    emit bloom();
}

