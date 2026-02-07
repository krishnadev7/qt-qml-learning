#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

class Backend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double pressure READ pressure NOTIFY pressureChanged)

public:
    explicit Backend(QObject *parent = nullptr);

    double pressure() const;

public slots:
    void increasePressure();

signals:
    void pressureChanged();

private:
    double m_pressure;
};

#endif
