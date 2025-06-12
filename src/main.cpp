#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "PolygonLoader.h"
#include "model/AircraftModel.h"
#include "services/FlightDatabaseManager.h"
#include "model/FlightModel.h"
#include "model/PolygonModel.h"
#include <QGeoCoordinate>
#include <QTimer>
#include <QDebug>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:qml/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    PolygonLoader loader;
    AircraftModel aircraftModel;
    PolygonModel polygonModel;
    //FlightModel flightModel;
    QList<QGeoCoordinate> hanoiPolygon = loader.loadHanoiPolygon("/home/domanhcuong/development/C++/QT/aircraft_hanoi/data/hanoi.json");
    aircraftModel.setPolygonModel(&polygonModel);
    aircraftModel.loadDataAsync();
    engine.rootContext()->setContextProperty("hanoiPolygon", QVariant::fromValue(hanoiPolygon));
    engine.rootContext()->setContextProperty("aircraftModel", &aircraftModel);
    engine.rootContext()->setContextProperty("polygonModel", &polygonModel);
    engine.load(url);
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        aircraftModel.updatePositions();
    });
    timer.start(1000);
    return app.exec();
}
