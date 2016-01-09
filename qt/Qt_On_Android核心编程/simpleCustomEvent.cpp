#include "simpleCustomEvent.h"

QEvent::Type SimpleCustomEvent::m_evType = (QEvent::Type)QEvent::None;

SimpleCustomEvent::SimpleCustomEvent(int arg1, const QString &arg2)
    : QEvent(eventType()), m_arg1(arg1), m_arg2(arg2)
{}

SimpleCustomEvent::~SimpleCustomEvent()
{

}

QEvent::Type SimpleCustomEvent::eventType()
{
    if(m_evType == QEvent::None)
    {
        m_evType = (QEvent::Type)registerEventType();
    }
    return m_evType;
}


