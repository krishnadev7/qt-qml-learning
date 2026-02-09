#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "backend/plantcontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Backend backend;

    qmlRegisterUncreatableType<Backend>(
        "WaterDashboard", 1, 0, "PlantController",
        "PlantController enum only; instance is provided as context property 'plant'"
        );

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("plant", &plant);
    engine.loadFromModule("WaterDashboard", "Main");

    return app.exec();
}
