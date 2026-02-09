#include "plantcontroller.h"
#include <QTimer>
#include <QtMath>

PlantController::PlantController(QObject *parent)
    : QObject(parent)
{
    setupStateMachine();

    // Demo simulation timer (replaces your QML Timer)
    m_simTimer = new QTimer(this);
    m_simTimer->setInterval(50);
    connect(m_simTimer, &QTimer::timeout, this, [this]() { tickSim(); });
    m_simTimer->start();

    // One-shot timers to finish sequences
    m_backwashTimer = new QTimer(this);
    m_backwashTimer->setSingleShot(true);
    connect(m_backwashTimer, &QTimer::timeout, this, &PlantController::backwashFinished);

    m_cipTimer = new QTimer(this);
    m_cipTimer->setSingleShot(true);
    connect(m_cipTimer, &QTimer::timeout, this, &PlantController::cipFinished);
}

// -------------------- QML requests (events) --------------------
void PlantController::requestAuto()     { emit autoRequested(); }
void PlantController::requestManual()   { emit manualRequested(); }
void PlantController::requestBackwash() { emit backwashRequested(); }
void PlantController::requestCip()      { emit cipRequested(); }

// -------------------- Alarm ACK (demo) --------------------
void PlantController::ackAlarm(const QString &code)
{
    // Demo: remove one alarm code from list
    auto next = m_activeAlarms;
    next.removeAll(code);
    if (next != m_activeAlarms) {
        m_activeAlarms = next;
        emit alarmsChanged();
    }
}

// -------------------- State Machine Setup --------------------
void PlantController::setupStateMachine()
{
    m_autoState = new QState(&m_machine);
    m_manualState = new QState(&m_machine);
    m_backwashState = new QState(&m_machine);
    m_cipState = new QState(&m_machine);

    // Entry actions (THIS is where "what happens in a mode" lives)
    connect(m_autoState, &QState::entered, this, [this](){ enterAuto(); });
    connect(m_manualState, &QState::entered, this, [this](){ enterManual(); });
    connect(m_backwashState, &QState::entered, this, [this](){ enterBackwash(); });
    connect(m_cipState, &QState::entered, this, [this](){ enterCip(); });

    // Allowed transitions
    m_autoState->addTransition(this, &PlantController::manualRequested,   m_manualState);
    m_autoState->addTransition(this, &PlantController::backwashRequested, m_backwashState);
    m_autoState->addTransition(this, &PlantController::cipRequested,      m_cipState);

    m_manualState->addTransition(this, &PlantController::autoRequested, m_autoState);

    // sequences return to AUTO when finished OR when Auto requested
    m_backwashState->addTransition(this, &PlantController::backwashFinished, m_autoState);
    m_backwashState->addTransition(this, &PlantController::autoRequested,    m_autoState);

    m_cipState->addTransition(this, &PlantController::cipFinished, m_autoState);
    m_cipState->addTransition(this, &PlantController::autoRequested, m_autoState);

    m_machine.setInitialState(m_autoState);
    m_machine.start();
}

// -------------------- Entry handlers (real effects) --------------------
void PlantController::enterAuto()
{
    if (m_mode != Auto) {
        m_mode = Auto;
        emit modeChanged();
    }
    // In real system: enable normal filtration, close backwash valves etc.
}

void PlantController::enterManual()
{
    if (m_mode != Manual) {
        m_mode = Manual;
        emit modeChanged();
    }
    // In real system: lock auto sequences, allow manual commands
}

void PlantController::enterBackwash()
{
    if (m_mode != Backwash) {
        m_mode = Backwash;
        emit modeChanged();
    }
    // In real system: stop feed pump, open backwash valve, etc.
    // Demo sequence: finish after 4 seconds
    m_backwashTimer->start(4000);
}

void PlantController::enterCip()
{
    if (m_mode != Cip) {
        m_mode = Cip;
        emit modeChanged();
    }
    // In real system: CIP steps, interlocks, etc.
    // Demo sequence: finish after 6 seconds
    m_cipTimer->start(6000);
}

// -------------------- Simulation tick (later: Modbus updates) --------------------
void PlantController::tickSim()
{
    // Simulate tank movement
    m_tankLevel += 0.2;
    if (m_tankLevel > 100.0) m_tankLevel = 0.0;
    emit tankLevelChanged();

    // Simulate pressures/flows differently based on mode (so clicking DOES something visible)
    if (m_mode == Auto) {
        m_feedFlow = 18.0 + (m_tankLevel * 0.02);
        m_inletPsi = 2.6 + (m_tankLevel * 0.005);
        m_outletPsi = 1.9 + (m_tankLevel * 0.003);
    } else if (m_mode == Manual) {
        m_feedFlow = 10.0 + (m_tankLevel * 0.01);
        m_inletPsi = 2.2;
        m_outletPsi = 1.7;
    } else if (m_mode == Backwash) {
        // Backwash: lower feed, different pressure profile
        m_feedFlow = 6.0;
        m_inletPsi = 1.4;
        m_outletPsi = 1.2;
    } else { // CIP
        m_feedFlow = 0.0;
        m_inletPsi = 0.8;
        m_outletPsi = 0.8;
    }

    emit feedFlowChanged();
    emit inletPsiChanged();
    emit outletPsiChanged();

    updateDerived();     // TMP etc
    evaluateAlarms();    // alarms from values
}

void PlantController::updateDerived()
{
    const double newTmp = m_inletPsi - m_outletPsi;
    if (!qFuzzyCompare(m_tmp, newTmp)) {
        m_tmp = newTmp;
        emit tmpChanged();
    }
}

void PlantController::evaluateAlarms()
{
    QStringList next;

    // Demo alarms:
    // 1) High tank level
    if (m_tankLevel > 90.0) next << "HIGH_TANK_LEVEL";

    // 2) High TMP alarm (simulate fouling)
    if (m_tmp > 0.9) next << "HIGH_TMP";

    // 3) “Manual mode active” as a warning (optional)
    if (m_mode == Manual) next << "MANUAL_MODE_ACTIVE";

    if (next != m_activeAlarms) {
        m_activeAlarms = next;
        emit alarmsChanged();

        // Example: if HIGH_TMP occurs in AUTO, request backwash automatically
        const bool highTmp = m_activeAlarms.contains("HIGH_TMP");
        if (highTmp && m_mode == Auto) {
            emit backwashRequested();
        }
    }
}
