#include "backend.h"

Backend::Backend(QObject *parent)
    : QObject(parent), m_pressure(2.0)
{
}

double Backend::pressure() const
{
    return m_pressure;
}

void Backend::increasePressure()
{
    m_pressure += 0.1;
    emit pressureChanged();
}
