#include "backend.h"

Backend::Backend(QObject *parent)
    : QObject(parent), m_pressure(0.0)
{
}

double Backend::pressure() const
{
    return m_pressure;
}

void Backend::updatePressure()
{
    m_pressure += 5.0;
    if (m_pressure > 100)
        m_pressure = 0;

    emit pressureChanged();
}
