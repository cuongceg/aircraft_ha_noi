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

bool FlightDatabaseManager::insertFlight(const Flight &flight) {
    QString queryStr = R"(
        INSERT INTO flight (plan_id, start_point, end_point, in_time, out_time, routes)
        VALUES (:plan_id,
                ST_GeomFromText(:start_wkt, 4326),
                ST_GeomFromText(:end_wkt, 4326),
                :in_time, :out_time,
                ST_GeomFromText(:routes_wkt, 4326))
    )";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":plan_id", flight.planId);
    query.bindValue(":start_wkt", geoCoordinateToWKT(flight.startPoint));
    query.bindValue(":end_wkt", geoCoordinateToWKT(flight.endPoint));
    query.bindValue(":in_time", flight.inTime);
    query.bindValue(":out_time", flight.outTime);
    query.bindValue(":routes_wkt", routeToWKTLineString(flight.route));

    if (!query.exec()) {
        qWarning() << "Insert flight failed:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<Flight> FlightDatabaseManager::loadAllFlights() {
    QList<Flight> flights;

    QSqlQuery query("SELECT id, plan_id, ST_AsText(start_point), ST_AsText(end_point), in_time, out_time, ST_AsText(routes) FROM flight");
    while (query.next()) {
        Flight flight;
        flight.id = query.value(0).toInt();
        flight.planId = query.value(1).toString();
        flight.startPoint = parseWKTPoint(query.value(2).toString());
        flight.endPoint = parseWKTPoint(query.value(3).toString());
        flight.inTime = query.value(4).toDateTime();
        flight.outTime = query.value(5).toDateTime();
        flight.route = parseWKTLineString(query.value(6).toString());
        flights.append(flight);
    }

    return flights;
}

QList<Flight> FlightDatabaseManager::searchFlights(const QString &keyword, const QDate &date) {
    QList<Flight> flights;

    QString queryStr = R"(
        SELECT id, plan_id, ST_AsText(start_point), ST_AsText(end_point),
               in_time, out_time, ST_AsText(routes)
        FROM flight
        WHERE plan_id ILIKE :keyword
    )";

    if (date.isValid()) {
        queryStr += " AND DATE(in_time) = :date";
    }

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":keyword", "%" + keyword + "%");
    if (date.isValid()) {
        query.bindValue(":date", date);
    }

    if (!query.exec()) {
        qWarning() << "Flight query failed:" << query.lastError().text();
        return flights;
    }

    while (query.next()) {
        Flight flight;
        flight.id = query.value(0).toInt();
        flight.planId = query.value(1).toString();
        flight.startPoint = parseWKTPoint(query.value(2).toString());
        flight.endPoint = parseWKTPoint(query.value(3).toString());
        flight.inTime = query.value(4).toDateTime();
        flight.outTime = query.value(5).toDateTime();
        flight.route = parseWKTLineString(query.value(6).toString());
        flights.append(flight);
    }

    return flights;
}


QGeoCoordinate FlightDatabaseManager::parseWKTPoint(const QString &wkt) {
    QRegularExpression re("POINT\\(([-\\d\\.]+) ([-\\d\\.]+)\\)");
    auto match = re.match(wkt);
    if (match.hasMatch())
        return QGeoCoordinate(match.captured(2).toDouble(), match.captured(1).toDouble());
    return {};
}

QList<QGeoCoordinate> FlightDatabaseManager::parseWKTLineString(const QString &wkt) {
    QList<QGeoCoordinate> coordinates;
    if (!wkt.startsWith("LINESTRING(") || !wkt.endsWith(")"))
        return coordinates;
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
                coordinates.append(QGeoCoordinate(lat, lon));
            }
        }
    }

    return coordinates;
}

QString FlightDatabaseManager::geoCoordinateToWKT(const QGeoCoordinate &coord) {
    return QString("POINT(%1 %2)").arg(coord.longitude(), 0, 'f', 8).arg(coord.latitude(), 0, 'f', 8);
}

QString FlightDatabaseManager::routeToWKTLineString(const QList<QGeoCoordinate> &route) {
    QStringList pointStrs;
    for (const auto &coord : route)
        pointStrs << QString("%1 %2").arg(coord.longitude(), 0, 'f', 8).arg(coord.latitude(), 0, 'f', 8);
    return QString("LINESTRING(%1)").arg(pointStrs.join(", "));
}
