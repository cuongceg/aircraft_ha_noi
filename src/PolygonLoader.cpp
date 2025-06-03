#include "PolygonLoader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

PolygonLoader::PolygonLoader(QObject *parent) : QObject(parent) {}

QList<QGeoCoordinate> PolygonLoader::loadHanoiPolygon(const QString &filePath) {
    QList<QGeoCoordinate> coords;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Failed to open file");
        return coords;
    }

    QByteArray data = file.readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        qWarning("JSON parse error: %s", qPrintable(parseError.errorString()));
        return coords;
    }

    QJsonObject obj = doc.object();
    if (obj["type"].toString() != "Polygon") {
        qWarning("Not a Polygon");
        return coords;
    }

    QJsonArray outerRing = obj["coordinates"].toArray().first().toArray();
    for (const QJsonValue &val : outerRing) {
        QJsonArray lonlat = val.toArray();
        double lon = lonlat.at(0).toDouble();
        double lat = lonlat.at(1).toDouble();
        coords.append(QGeoCoordinate(lat, lon));
    }

    return coords;
}
