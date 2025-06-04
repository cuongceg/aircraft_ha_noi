#include "AirCraftManager.h"
#include "GeoUtils.h"
#include <QtMath>

AircraftModel::AircraftModel(QObject *parent) : QAbstractListModel(parent)
{
    m_pathPoints.append(QGeoCoordinate(20.67, 106.05));   // start (Vinh Bắc Bộ)
    m_pathPoints.append(QGeoCoordinate(21.0, 105.8));   // end (Hà Nội)
    double rotation = qAtan2(
                            m_pathPoints[1].longitude()  - m_pathPoints[0].longitude(),
                            m_pathPoints[1].latitude()  - m_pathPoints[0] .latitude()
                            ) * 180.0 / M_PI;
    m_aircrafts.append({ m_pathPoints[0],rotation });
    m_progress.append(0.0);
}

int AircraftModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_aircrafts.count();
}

QVariant AircraftModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_aircrafts.count())
        return QVariant();

    const AircraftData &aircraft = m_aircrafts.at(index.row());

    if (role == CoordinateRole)
        return QVariant::fromValue(aircraft.coordinate);
    if (role == InsideHanoiRole)
        return QVariant(aircraft.isInsideHanoi);
    if (role == RotationRole)
        return QVariant(aircraft.rotation);
    return QVariant();
}

QHash<int, QByteArray> AircraftModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CoordinateRole] = "coordinate";
    roles[InsideHanoiRole] = "insideHanoi";
    roles[RotationRole] = "rotation";
    return roles;
}

void AircraftModel::updatePositions()
{
    for (int i = 0; i < m_aircrafts.size(); ++i) {
        m_progress[i] += 0.01;
        if (m_progress[i] > 1.0)
            m_progress[i] = 0.0;

        // linear interpolation
        const QGeoCoordinate &start = m_pathPoints[0];
        const QGeoCoordinate &end = m_pathPoints[1];

        QGeoCoordinate newPos(
            start.latitude() + (end.latitude() - start.latitude()) * m_progress[i],
            start.longitude() + (end.longitude() - start.longitude()) * m_progress[i]
        );

        if (m_aircrafts[i].coordinate != newPos) {
            m_aircrafts[i].coordinate = newPos;
            if(isPointInPolygon(newPos,m_hanoiPolygon)){
                m_aircrafts[i].isInsideHanoi = true;
                qDebug() << "Aircraft is inside Hanoi";
            }else{
                qDebug() << "Outside Hanoi";
            }
            // emit change
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, {CoordinateRole,InsideHanoiRole,RotationRole});
        }
    }
}

void AircraftModel::setPolygon(QList<QGeoCoordinate>& hanoiPolygon){
    m_hanoiPolygon = hanoiPolygon;
}
