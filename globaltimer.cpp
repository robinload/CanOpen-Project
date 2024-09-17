#include "globaltimer.h"

GlobalTimer* GlobalTimer::m_instance = nullptr;

GlobalTimer* GlobalTimer::instance()
{
    if (!m_instance)
    {
        m_instance = new GlobalTimer();
    }
    return m_instance;
}

GlobalTimer::GlobalTimer(QObject *parent) : QObject(parent)
{
    m_timer.setInterval(10); // 10 ms interval
}

QTimer* GlobalTimer::getTimer()
{
    return &m_timer;
}
