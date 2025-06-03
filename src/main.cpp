#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "PolygonLoader.h"
#include "AirCraftManager.h"
#include <QGeoCoordinate>
#include <QTimer>

int main(int argc, char *argv[])
{
qRegisterMetaType<QGeoCoordinate>("QGeoCoordinate");
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
    QList<QGeoCoordinate> hanoiPolygon = loader.loadHanoiPolygon("/home/domanhcuong/development/C++/QT/aircraft_hanoi/data/hanoi.json");
    engine.rootContext()->setContextProperty("hanoiPolygon", QVariant::fromValue(hanoiPolygon));
    engine.rootContext()->setContextProperty("aircraftModel", &aircraftModel);
    engine.load(url);

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        aircraftModel.updatePositions();
    });
    timer.start(1000);
    return app.exec();
}
