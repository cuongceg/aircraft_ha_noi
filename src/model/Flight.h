#ifndef FLIGHT_H
#define FLIGHT_H
#include <QString>
#include <QGeoCoordinate>
#include <QDateTime>

struct WayPoint{
    double latitude;
    double longitude;
    WayPoint(double lat,double longi){
        latitude = lat;
        longitude = longi;
    }
};

struct FlightData {
    int id;
    QString planeId;
    QDateTime startTime;
    QDateTime endTime;
    QVector<WayPoint> path;
};
#endif // FLIGHT_H
