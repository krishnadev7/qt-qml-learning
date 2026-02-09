#include "backend.h"
#include <QTimer>

Backend::Backend(QObject *parent)
    : QObject(parent)
{
    // simulate changing values (later: Modbus updates these)
    auto *t = new QTimer(this);
    t->setInterval(50);
    connect(t, &QTimer::timeout, this, [this]() { tickSim(); });
    t->start();
}

void Backend::tickSim()
{
    // tank level
    m_tankLevel += 0.2;
    if (m_tankLevel > 100.0) m_tankLevel = 0.0;
    emit tankLevelChanged();

    // small dummy drift
    m_feedFlow = 18.0 + (m_tankLevel * 0.02);
    m_permeateFlow = 16.0 + (m_tankLevel * 0.015);
    emit feedFlowChanged();
    emit permeateFlowChanged();

    // simple alarm demo (we’ll upgrade this in Step 4)
    const bool highLevel = (m_tankLevel > 90.0);
    QStringList next;
    if (highLevel) next << "HIGH_TANK_LEVEL";

    if (next != m_activeAlarms) {
        m_activeAlarms = next;
        emit alarmsChanged();
    }
}

// Requests (for now just set directly; Step 3 will route via state machine)
void Backend::requestAuto()     { if (m_mode != Auto)     { m_mode = Auto;     emit modeChanged(); } }
void Backend::requestManual()   { if (m_mode != Manual)   { m_mode = Manual;   emit modeChanged(); } }
void Backend::requestBackwash() { if (m_mode != Backwash) { m_mode = Backwash; emit modeChanged(); } }
void Backend::requestCip()      { if (m_mode != Cip)      { m_mode = Cip;      emit modeChanged(); } }
