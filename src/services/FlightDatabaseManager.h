#ifndef FLIGHTDATABASEMANAGER_H
#define FLIGHTDATABASEMANAGER_H
#include <QObject>
#include <QSqlDatabase>
#include <QGeoCoordinate>
#include "../model/Flight.h"
class FlightDatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit FlightDatabaseManager(QObject *parent = nullptr);
    ~FlightDatabaseManager();

    bool connectToDatabase(const QString &host, int port,
                           const QString &dbName,
                           const QString &user, const QString &password);

    bool insertFlight(const QString &planeId, const QDateTime &startTime, const QDateTime &endTime, const QString &pathWKT);
    QVector<FlightData> getAllFlights();
    //QList<Flight> searchFlights(const QString &keyword, const QDate &date);

private:
    QSqlDatabase m_db;
    QVector<WayPoint> parseWKTLineString(const QString &wkt);
};
#endif // FLIGHTDATABASEMANAGER_H
