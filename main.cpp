#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QGeoCoordinate>
#include <QQmlContext>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    QList<QGeoCoordinate> hanoiPolygon = {
        {21.0, 105.8},
        {21.0, 105.9},
        {21.1, 105.9},
        {21.1, 105.8},
        {21.0, 105.8}
    };
    engine.rootContext()->setContextProperty("hanoiPolygon", QVariant::fromValue(hanoiPolygon));
    engine.load(url);

    return app.exec();
}
