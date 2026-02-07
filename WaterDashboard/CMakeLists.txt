cmake_minimum_required(VERSION 3.16)

project(WaterDashboard LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appWaterDashboard
    main.cpp
    backend/backend.cpp
    backend/backend.h
)

qt_add_qml_module(appWaterDashboard
    URI WaterDashboard
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/Dashboard.qml
        qml/components/Pipe.qml
        qml/components/Tank.qml
        QML_FILES qml/components/StatusCard.qml
        QML_FILES qml/components/SymbolPump.qml
        QML_FILES qml/components/SymbolValve.qml
)

target_link_libraries(appWaterDashboard
    PRIVATE Qt6::Quick
)
