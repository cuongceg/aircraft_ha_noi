#ifndef FLIGHT_H
#define FLIGHT_H
#include <QString>
#include <QGeoCoordinate>
#include <QDateTime>

struct Flight {
    int id = -1; // giờ là số nguyên
    QString planId;
    QGeoCoordinate startPoint;
    QGeoCoordinate endPoint;
    QDateTime inTime;
    QDateTime outTime;
    QList<QGeoCoordinate> route;
};
#endif // FLIGHT_H
