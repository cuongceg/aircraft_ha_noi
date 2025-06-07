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

bool FlightDatabaseManager::insertFlight(const AircraftData &flight) {
    QString queryStr = R"(
        INSERT INTO flight (id, plan_id, start_point, end_point, in_time, out_time, routes)
        VALUES (:id, :plan_id,
                ST_GeomFromText(:start_wkt, 4326),
                ST_GeomFromText(:end_wkt, 4326),
                :in_time, :out_time,
                ST_GeomFromText(:routes_wkt, 4326))
    )";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":id", flight.id);
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

QList<AircraftData> FlightDatabaseManager::loadAllFlights() {
    QList<AircraftData> flights;

    QSqlQuery query("SELECT id, plan_id, ST_AsText(start_point), ST_AsText(end_point), in_time, out_time, ST_AsText(routes) FROM flight");
    while (query.next()) {
        AircraftData flight;
        flight.id = query.value(0).toString();
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
    QList<QGeoCoordinate> coords;
    QRegularExpression re("LINESTRING\\(([^\\)]+)\\)");
    auto match = re.match(wkt);
    if (match.hasMatch()) {
        QStringList pairs = match.captured(1).split(", ");
        for (const QString &pair : pairs) {
            auto parts = pair.split(" ");
            if (parts.size() == 2)
                coords.append(QGeoCoordinate(parts[1].toDouble(), parts[0].toDouble()));
        }
    }
    return coords;
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
