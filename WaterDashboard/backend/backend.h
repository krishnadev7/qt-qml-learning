#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QStringList>

class Backend : public QObject
{
    Q_OBJECT

public:
    enum Mode { Auto, Manual, Backwash, Cip };
    Q_ENUM(Mode)

    Q_PROPERTY(double tankLevel READ tankLevel NOTIFY tankLevelChanged)
    Q_PROPERTY(double feedFlow READ feedFlow NOTIFY feedFlowChanged)
    Q_PROPERTY(double permeateFlow READ permeateFlow NOTIFY permeateFlowChanged)
    Q_PROPERTY(Mode mode READ mode NOTIFY modeChanged)

    Q_PROPERTY(QStringList activeAlarms READ activeAlarms NOTIFY alarmsChanged)

public:
    explicit Backend(QObject *parent = nullptr);

    double tankLevel() const { return m_tankLevel; }
    double feedFlow() const { return m_feedFlow; }
    double permeateFlow() const { return m_permeateFlow; }
    Mode mode() const { return m_mode; }
    QStringList activeAlarms() const { return m_activeAlarms; }

public slots:
    // QML calls these (requests)
    void requestAuto();
    void requestManual();
    void requestBackwash();
    void requestCip();

signals:
    void tankLevelChanged();
    void feedFlowChanged();
    void permeateFlowChanged();
    void modeChanged();
    void alarmsChanged();

private:
    // Step 1: simulation in C++ (later replaced by Modbus)
    void tickSim();

    double m_tankLevel = 0.0;
    double m_feedFlow = 18.2;
    double m_permeateFlow = 16.8;

    Mode m_mode = Auto;

    QStringList m_activeAlarms;
};

#endif
