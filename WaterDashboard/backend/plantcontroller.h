#ifndef PLANTCONTROLLER_H
#define PLANTCONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QStateMachine>
#include <QState>

class PlantController : public QObject
{
    Q_OBJECT

public:
    enum Mode { Auto, Manual, Backwash, Cip };
    Q_ENUM(Mode)

    // Live values for QML
    Q_PROPERTY(double tankLevel READ tankLevel NOTIFY tankLevelChanged)
    Q_PROPERTY(double feedFlow READ feedFlow NOTIFY feedFlowChanged)
    Q_PROPERTY(double inletPsi READ inletPsi NOTIFY inletPsiChanged)
    Q_PROPERTY(double outletPsi READ outletPsi NOTIFY outletPsiChanged)
    Q_PROPERTY(double tmp READ tmp NOTIFY tmpChanged)

    // Mode + alarms
    Q_PROPERTY(Mode mode READ mode NOTIFY modeChanged)
    Q_PROPERTY(QStringList activeAlarms READ activeAlarms NOTIFY alarmsChanged)

public:
    explicit PlantController(QObject *parent = nullptr);

    // getters
    double tankLevel() const { return m_tankLevel; }
    double feedFlow() const { return m_feedFlow; }
    double inletPsi() const { return m_inletPsi; }
    double outletPsi() const { return m_outletPsi; }
    double tmp() const { return m_tmp; }

    Mode mode() const { return m_mode; }
    QStringList activeAlarms() const { return m_activeAlarms; }

    // QML requests (do NOT directly set mode here)
    Q_INVOKABLE void requestAuto();
    Q_INVOKABLE void requestManual();
    Q_INVOKABLE void requestBackwash();
    Q_INVOKABLE void requestCip();

    // Alarm ack (demo)
    Q_INVOKABLE void ackAlarm(const QString &code);

signals:
    // property notify
    void tankLevelChanged();
    void feedFlowChanged();
    void inletPsiChanged();
    void outletPsiChanged();
    void tmpChanged();

    void modeChanged();
    void alarmsChanged();

    // state-machine events
    void autoRequested();
    void manualRequested();
    void backwashRequested();
    void cipRequested();

    void backwashFinished();
    void cipFinished();

private:
    void setupStateMachine();
    void updateDerived();
    void evaluateAlarms();

    // state entry handlers
    void enterAuto();
    void enterManual();
    void enterBackwash();
    void enterCip();

    // demo simulation tick (later replace with Modbus)
    void tickSim();

private:
    // data
    double m_tankLevel = 0.0;
    double m_feedFlow = 18.2;
    double m_inletPsi = 2.6;
    double m_outletPsi = 1.9;
    double m_tmp = 0.7;

    Mode m_mode = Auto;

    QStringList m_activeAlarms;

    // state machine
    QStateMachine m_machine;
    QState *m_autoState = nullptr;
    QState *m_manualState = nullptr;
    QState *m_backwashState = nullptr;
    QState *m_cipState = nullptr;

    // demo timers (owned via parent)
    class QTimer *m_simTimer = nullptr;
    class QTimer *m_backwashTimer = nullptr;
    class QTimer *m_cipTimer = nullptr;
};

#endif
