#ifndef AIRCRAFTMANAGER_H
#define AIRCRAFTMANAGER_H
// Aircraft.h
#pragma once
#include <QAbstractListModel>
#include <QGeoCoordinate>

struct AircraftData {
    QGeoCoordinate coordinate;
};

class AircraftModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        CoordinateRole = Qt::UserRole + 1,
    };

    explicit AircraftModel(QObject *parent = nullptr);

    // Override
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void updatePositions();

private:
    QList<AircraftData> m_aircrafts;
    QList<QGeoCoordinate> m_pathPoints; // điểm start->end
    QList<double> m_progress; // tiến độ di chuyển của từng máy bay
};

#endif // AIRCRAFT_H
