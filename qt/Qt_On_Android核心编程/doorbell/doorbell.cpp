#include "doorbell.h"
#include <QDateTime>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QMovie>
#include <QScopedPointer>
#include <QDebug>

QEvent::Type BellEvent::m_evType = QEvent::None;
BellEvent::BellEvent(const QString &visitor,
                     const QStringList &gifts)
    : QEvent(eventType()), m_visitor(visitor)
    , m_gifts(gifts)
{
}

QEvent::Type BellEvent::eventType()
{
    if(m_evType == QEvent::None)
    {
        m_evType = (QEvent::Type)registerEventType();
    }
    return m_evType;
}

//
BellThread::BellThread(QObject *receiver)
    : m_receiver(receiver)
{
}

void BellThread::run()
{
    qDebug() << "BellThread, tid - "
             << QThread::currentThreadId();
    QScopedPointer<Ringer> firer(new Ringer(this));
    exec();
}

const char * g_vistors[5] = {
    "Mary", "Peter", "John",
    "Haali", "Mike"
};

const char * g_gifts[7] = {
    "apple", "iPad mini 2", "banana",
    "egg", "iPhone 6", "Bambook",
    "Kindle Fire HD"
};

Ringer::Ringer(BellThread *t) : m_thread(t)
{
    QTimer::singleShot(3000, this, SLOT(onTimeout()));
}

void Ringer::onTimeout()
{
    if(m_thread->m_receiver.isNull())
    {
        m_thread->quit();
        return;
    }

    qsrand(QDateTime::currentDateTime().toTime_t());
    QStringList gifts;
    gifts << g_gifts[ qrand() % 7 ];
    gifts << g_gifts[ qrand() % 7 ];
    BellEvent * ev = new BellEvent(
                g_vistors[ qrand()%5 ], gifts);
    QCoreApplication::postEvent(
                m_thread->m_receiver, ev);

    QTimer::singleShot(( qrand() % 8 + 2 ) * 1000
                       , this, SLOT(onTimeout()));
}

Master::Master(QWidget *parent)
    : QWidget(parent), m_visitorLabel(new QLabel)
    , m_movie(new QLabel)
{
    BellThread * t = new BellThread(this);
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    t->start();

    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(m_visitorLabel);

    QMovie * gif = new QMovie(":/eye.gif");
    m_movie->setMovie(gif);
    layout->addWidget(m_movie);
    m_movie->setVisible(false);
    layout->addStretch(1);

    setLayout(layout);
}

Master::~Master()
{

}

bool Master::event(QEvent *e)
{
    if(e->type() == BellEvent::eventType() )
    {
        BellEvent *ev = (BellEvent*)e;
        QString str = QString("%1, gitfs[ %2 ]")
                .arg(ev->m_visitor)
                .arg(ev->m_gifts.join(";"));
        m_visitorLabel->setText(str);
        if(ev->m_gifts.contains("iPhone 6")
                || ev->m_gifts.contains("Kindle Fire HD"))
        {
            m_movie->setVisible(true);
            m_movie->movie()->start();
        }
        else
        {
            m_movie->setVisible(false);
            m_movie->movie()->stop();
        }
        return true;
    }
    return QWidget::event(e);
}
