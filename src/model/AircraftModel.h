#ifndef AIRCRAFTMANAGER_H
#define AIRCRAFTMANAGER_H
// Aircraft.h
#pragma once
#include <QAbstractListModel>
#include <QGeoCoordinate>
#include<QDateTime>
#include<QDebug>
#include <QtConcurrent/QtConcurrent>
#include"PolygonModel.h"
#include "../services/FlightDatabaseManager.h"

struct AircraftData {
    QString icao24;
    QDateTime startTime;
    QDateTime endTime;
    QGeoCoordinate current;
    QVector<WayPoint>path;
    double rotation =0.0;
    bool isInsideHanoi=false;
};


class AircraftModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        CoordinateRole = Qt::UserRole + 1,
        InsideHanoiRole,
        RouteRole,
        StartPointRole,
        EndPointRole,
        RotationRole,
        StartTimeRole,
        EndTimeRole,
        PlaneIdRole
    };

    explicit AircraftModel(QObject *parent = nullptr);

    // Override
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void updatePositions();
    Q_INVOKABLE void startAircraft(int index);
    Q_INVOKABLE void stopAircraft(int index);
    Q_INVOKABLE void resetProgress(int index);
    Q_INVOKABLE void updateFlight(int index, const QVariantList &pathList);
    Q_INVOKABLE void addAircraft(QString icao24,const QVariantList &pathList);
    void loadDataAsync();
    void setPolygonModel(PolygonModel* polygonModel);

private:
    PolygonModel* m_polygonModel = nullptr;
    QVector<AircraftData> m_aircrafts;
    QList<int> m_progress;
    QVector<bool> m_running;
    AircraftData convertToAircraftData(const FlightData &flight);
};

#endif // AIRCRAFT_H
