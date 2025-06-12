// #include <QFile>
// #include <QJsonDocument>
// #include <QJsonObject>
// #include <QJsonArray>
// #include <QDateTime>
// #include <QDebug>
// #include "FlightLoader.h"

// FlightLoader::FlightLoader(QObject *parent) : QObject(parent) {}
// AircraftData FlightLoader::loadAircraftFromJson(const QString &filePath) {
//     AircraftData aircraft;

//     QFile file(filePath);
//     if (!file.open(QIODevice::ReadOnly)) {
//         qWarning() << "Could not open file:" << filePath;
//         return aircraft;
//     }

//     QByteArray jsonData = file.readAll();
//     file.close();

//     QJsonParseError parseError;
//     QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
//     if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
//         qWarning() << "Invalid JSON format:" << parseError.errorString();
//         return aircraft;
//     }

//     QJsonObject obj = doc.object();

//     aircraft.icao24 = obj.value("icao24").toString();

//     qint64 startEpoch = static_cast<qint64>(obj.value("startTime").toDouble());
//     qint64 endEpoch = static_cast<qint64>(obj.value("endTime").toDouble());

//     aircraft.startTime = QDateTime::fromSecsSinceEpoch(startEpoch);
//     aircraft.endTime = QDateTime::fromSecsSinceEpoch(endEpoch);

//     QJsonArray pathArray = obj.value("path").toArray();
//     for (const QJsonValue &val : pathArray) {
//         QJsonArray point = val.toArray();
//         if (point.size() < 6)
//             continue;

//         WayPoint wp;
//         // wp.time = static_cast<qint64>(point[0].toDouble());
//         wp.latitude = point[1].toDouble();
//         wp.longitude = point[2].toDouble();
//         // wp.baroAltitude = point[3].toDouble();
//         // wp.trueTrack = point[4].toDouble();
//         // wp.onGround = point[5].toBool();

//         aircraft.path.append(wp);
//     }

//     // Gán current = waypoint đầu tiên nếu có
//     if (!aircraft.path.isEmpty()) {
//         const WayPoint &wp0 = aircraft.path.first();
//         aircraft.current = QGeoCoordinate(wp0.latitude, wp0.longitude);
//     }

//     return aircraft;
// }
