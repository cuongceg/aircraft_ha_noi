#ifndef AIRCRAFTMANAGER_H
#define AIRCRAFTMANAGER_H
// Aircraft.h
#pragma once
#include <QAbstractListModel>
#include <QGeoCoordinate>
#include<QDebug>

struct AircraftData {
    QGeoCoordinate coordinate;
    double rotation=0.0;
    bool isInsideHanoi=false;
};

class AircraftModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        CoordinateRole = Qt::UserRole + 1,
        InsideHanoiRole,
        RotationRole
    };

    explicit AircraftModel(QObject *parent = nullptr);

    // Override
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void updatePositions();
    void setPolygon(QList<QGeoCoordinate>& hanoiPolygon);

private:
    QList<AircraftData> m_aircrafts;
    QList<QGeoCoordinate> m_pathPoints;
    QList<double> m_progress;
    QList<QGeoCoordinate> m_hanoiPolygon;
};

#endif // AIRCRAFT_H
