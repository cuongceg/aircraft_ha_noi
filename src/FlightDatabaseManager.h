#ifndef FLIGHTDATABASEMANAGER_H
#define FLIGHTDATABASEMANAGER_H
#include <QObject>
#include <QSqlDatabase>
#include <QGeoCoordinate>
#include "Flight.h"
class FlightDatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit FlightDatabaseManager(QObject *parent = nullptr);
    ~FlightDatabaseManager();

    bool connectToDatabase(const QString &host, int port,
                           const QString &dbName,
                           const QString &user, const QString &password);

    bool insertFlight(const Flight &flight);
    QList<Flight> loadAllFlights();
    QList<Flight> searchFlights(const QString &keyword, const QDate &date);

private:
    QSqlDatabase m_db;

    QGeoCoordinate parseWKTPoint(const QString &wkt);
    QList<QGeoCoordinate> parseWKTLineString(const QString &wkt);
    QString geoCoordinateToWKT(const QGeoCoordinate &coord);
    QString routeToWKTLineString(const QList<QGeoCoordinate> &route);
};
#endif // FLIGHTDATABASEMANAGER_H
