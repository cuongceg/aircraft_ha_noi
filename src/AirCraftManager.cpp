#include "AirCraftManager.h"

AircraftModel::AircraftModel(QObject *parent) : QAbstractListModel(parent)
{
    // Khởi tạo 1 máy bay di chuyển từ điểm A tới điểm B
    m_pathPoints.append(QGeoCoordinate(20.0, 106.0));   // start (Vinh Bắc Bộ)
    m_pathPoints.append(QGeoCoordinate(21.0, 105.8));   // end (Hà Nội)

    // Ban đầu máy bay đứng tại start
    m_aircrafts.append({ m_pathPoints[0] });
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

    return QVariant();
}

QHash<int, QByteArray> AircraftModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CoordinateRole] = "coordinate";
    return roles;
}

void AircraftModel::updatePositions()
{
    // Cập nhật vị trí máy bay theo tiến độ
    bool changed = false;
    for (int i = 0; i < m_aircrafts.size(); ++i) {
        m_progress[i] += 0.01;
        if (m_progress[i] > 1.0)
            m_progress[i] = 0.0;

        // Tính tọa độ dựa trên linear interpolation
        const QGeoCoordinate &start = m_pathPoints[0];
        const QGeoCoordinate &end = m_pathPoints[1];

        QGeoCoordinate newPos(
            start.latitude() + (end.latitude() - start.latitude()) * m_progress[i],
            start.longitude() + (end.longitude() - start.longitude()) * m_progress[i]
            );

        if (m_aircrafts[i].coordinate != newPos) {
            m_aircrafts[i].coordinate = newPos;
            changed = true;

            // Phát tín hiệu dữ liệu thay đổi tại dòng i
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, {CoordinateRole});
        }
    }
}
