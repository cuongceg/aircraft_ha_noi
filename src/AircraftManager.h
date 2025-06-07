#ifndef AIRCRAFTMANAGER_H
#define AIRCRAFTMANAGER_H
// Aircraft.h
#pragma once
#include <QAbstractListModel>
#include <QGeoCoordinate>
#include<QDateTime>
#include<QDebug>

struct AircraftData {
    QString id;
    QString planId;
    QGeoCoordinate startPoint;
    QGeoCoordinate endPoint;
    QDateTime inTime;
    QDateTime outTime;
    QGeoCoordinate currentCoordinate;
    double rotation = 0.0;
    QList<QGeoCoordinate> route;
    bool isInsideHanoi = false;
};

class AircraftModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        CoordinateRole = Qt::UserRole + 1,
        InsideHanoiRole,
        RotationRole,
        StartPointRole,
        EndPointRole
    };

    explicit AircraftModel(QObject *parent = nullptr);

    // Override
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void updatePositions();
    Q_INVOKABLE void resetProgress(int index);
    Q_INVOKABLE void updateFlights(int index,QGeoCoordinate startPoint,QGeoCoordinate endPoint);
    void setPolygon(QList<QGeoCoordinate>& hanoiPolygon);
    void getStartAndEndPoint();

private:
    QList<AircraftData> m_aircrafts;
    QList<double> m_progress;
    QList<QGeoCoordinate> m_hanoiPolygon;
};

#endif // AIRCRAFT_H
