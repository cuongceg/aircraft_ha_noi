#include "FlightDatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QRegularExpression>

FlightDatabaseManager::FlightDatabaseManager(QObject *parent) : QObject(parent) {}

FlightDatabaseManager::~FlightDatabaseManager() {
    if (m_db.isOpen()) m_db.close();
}

bool FlightDatabaseManager::connectToDatabase(const QString &host, int port,
                                              const QString &dbName,
                                              const QString &user, const QString &password) {
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName(host);
    m_db.setPort(port);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(user);
    m_db.setPassword(password);

    if (!m_db.open()) {
        qWarning() << "Database connection failed:" << m_db.lastError().text();
        return false;
    }
    qDebug() << "Database connected.";
    return true;
}

bool FlightDatabaseManager::insertFlight(const QString &planeId, const QDateTime &startTime, const QDateTime &endTime, const QString &pathWKT) {
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO Flight (planeId, startTime, endTime, path)
        VALUES (:planeId, :startTime, :endTime, ST_GeomFromText(:pathWKT, 4326))
    )");

    query.bindValue(":planeId", planeId);
    query.bindValue(":startTime", startTime);
    query.bindValue(":endTime", endTime);
    query.bindValue(":pathWKT", pathWKT);

    if (!query.exec()) {
        qWarning() << "Insert failed:" << query.lastError().text();
        return false;
    }
    return true;
}

QVector<FlightData> FlightDatabaseManager::getAllFlights() {
    QVector<FlightData> flights;
    QSqlQuery query("SELECT id, planeId, startTime, endTime, ST_AsText(path) FROM Flight");

    while (query.next()) {
        FlightData flight;
        flight.id = query.value(0).toInt();
        flight.planeId = query.value(1).toString();
        flight.startTime = query.value(2).toDateTime();
        flight.endTime = query.value(3).toDateTime();
        flight.path = parseWKTLineString(query.value(4).toString());
        flights.append(flight);
    }
    return flights;
}

// QList<Flight> FlightDatabaseManager::searchFlights(const QString &keyword, const QDate &date) {
//     QList<Flight> flights;

//     QString queryStr = R"(
//         SELECT id, plan_id, ST_AsText(start_point), ST_AsText(end_point),
//                in_time, out_time, ST_AsText(routes)
//         FROM flight
//         WHERE plan_id ILIKE :keyword
//     )";

//     if (date.isValid()) {
//         queryStr += " AND DATE(in_time) = :date";
//     }

//     QSqlQuery query;
//     query.prepare(queryStr);
//     query.bindValue(":keyword", "%" + keyword + "%");
//     if (date.isValid()) {
//         query.bindValue(":date", date);
//     }

//     if (!query.exec()) {
//         qWarning() << "Flight query failed:" << query.lastError().text();
//         return flights;
//     }

//     while (query.next()) {
//         Flight flight;
//         flight.id = query.value(0).toInt();
//         flight.planId = query.value(1).toString();
//         flight.startPoint = parseWKTPoint(query.value(2).toString());
//         flight.endPoint = parseWKTPoint(query.value(3).toString());
//         flight.inTime = query.value(4).toDateTime();
//         flight.outTime = query.value(5).toDateTime();
//         flight.route = parseWKTLineString(query.value(6).toString());
//         flights.append(flight);
//     }

//     return flights;
// }
QVector<WayPoint> FlightDatabaseManager::parseWKTLineString(const QString &wkt) {
    QVector<WayPoint> wayPoints;
    if (!wkt.startsWith("LINESTRING(") || !wkt.endsWith(")"))
        return wayPoints;
    QString content = wkt.mid(QString("LINESTRING(").length());
    content.chop(1); // Bỏ dấu ")"
    QStringList coordPairs = content.split(",", Qt::SkipEmptyParts);
    for (const QString& pair : coordPairs) {
        QString trimmed = pair.trimmed();
        QStringList lonLat = trimmed.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
        if (lonLat.size() == 2) {
            bool lonOk, latOk;
            double lon = lonLat[0].toDouble(&lonOk);
            double lat = lonLat[1].toDouble(&latOk);
            if (lonOk && latOk) {
                wayPoints.append(WayPoint(lat, lon));
            }
        }
    }

    return wayPoints;
}
