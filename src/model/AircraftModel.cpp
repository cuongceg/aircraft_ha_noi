#include "AircraftModel.h"
#include "PolygonModel.h"
#include <QtMath>

AircraftModel::AircraftModel(QObject *parent) : QAbstractListModel(parent)
{}

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

    switch (role) {
    case CoordinateRole:
        return QVariant::fromValue(aircraft.current);
    case RotationRole:
        return QVariant(aircraft.rotation);
    case InsideHanoiRole:
        return QVariant(aircraft.isInsideHanoi);
    case RouteRole:{
        QVariantList pathList;
        for (const WayPoint &wp : aircraft.path) {
            pathList.append(QVariant::fromValue(QGeoCoordinate(wp.latitude, wp.longitude)));
        }
        return pathList;
    }
    case StartPointRole:{
        WayPoint start = aircraft.path.first();
        QGeoCoordinate startPoint = QGeoCoordinate(start.latitude,start.longitude);
        return QVariant::fromValue(startPoint);
    }
    case EndPointRole:{
        WayPoint start = aircraft.path.last();
        QGeoCoordinate startPoint = QGeoCoordinate(start.latitude,start.longitude);
        return QVariant::fromValue(startPoint);
    }
    case StartTimeRole:
        return QVariant(aircraft.startTime);
    case EndTimeRole:
        return QVariant(aircraft.endTime);
    case PlaneIdRole:
        return QVariant(aircraft.icao24);
    }
    return QVariant();
}

QHash<int, QByteArray> AircraftModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CoordinateRole] = "coordinate";
    roles[InsideHanoiRole] = "insideHanoi";
    roles[RouteRole] = "routes";
    roles[StartPointRole] = "startPoint";
    roles[EndPointRole] = "endPoint";
    roles[RotationRole] = "rotation";
    roles[StartTimeRole] = "startTime";
    roles[EndTimeRole] = "endTime";
    roles[PlaneIdRole] = "planeId";
    return roles;
}

void AircraftModel::updatePositions() {
    for (int i = 0; i < m_aircrafts.size(); ++i) {
        if (!m_running[i] || m_aircrafts[i].path.isEmpty())
            continue;

        int progress = static_cast<int>(m_progress.value(i, 0));
        if (progress < m_aircrafts[i].path.size()) {
            const WayPoint &wp = m_aircrafts[i].path[progress];
            if(progress>0){
                m_aircrafts[i].rotation = qAtan2(wp.longitude - m_aircrafts[i].current.longitude(),
                                                 wp.latitude  - m_aircrafts[i].current.latitude()) * 180.0 / M_PI;
            }
            m_aircrafts[i].current = QGeoCoordinate(wp.latitude, wp.longitude);
            m_progress[i] = progress + 1;
            if (m_polygonModel->isInsidePolygons(m_aircrafts[i].current) && !m_aircrafts[i].isInsideHanoi) {
                m_aircrafts[i].isInsideHanoi = true;
                qDebug() << "Aircraft" << m_aircrafts[i].icao24 << "entered Hanoi";
            } else if (!m_polygonModel->isInsidePolygons(m_aircrafts[i].current) && m_aircrafts[i].isInsideHanoi) {
                m_aircrafts[i].isInsideHanoi = false;
                qDebug() << "Aircraft" << m_aircrafts[i].icao24 << "left Hanoi";
            }
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, {CoordinateRole, InsideHanoiRole,RotationRole,StartTimeRole,EndTimeRole,PlaneIdRole});
        } else {
            m_running[i] = false;
        }
    }
}


void AircraftModel::startAircraft(int index) {
    if (index >= 0 && index < m_aircrafts.size()) {
        m_running[index] = true;
        emit dataChanged(this->index(index), this->index(index));
    }
}

void AircraftModel::stopAircraft(int index) {
    if (index >= 0 && index < m_aircrafts.size()) {
        m_running[index] = false;
        emit dataChanged(this->index(index), this->index(index));
    }
}

void AircraftModel::addAircraft(QString icao24,const QVariantList &pathList) {
    beginInsertRows(QModelIndex(), m_aircrafts.size(), m_aircrafts.size());
    QVector<WayPoint> wayPoint;
    for (const QVariant &point : pathList) {
        QVariantMap map = point.toMap();
        wayPoint.append(WayPoint(map["latitude"].toDouble(),map["longitude"].toDouble()));
    }
    const WayPoint &wp0 = wayPoint.first();
    QGeoCoordinate current = QGeoCoordinate(wp0.latitude, wp0.longitude);
    m_aircrafts.append({icao24,QDateTime::currentDateTime(),QDateTime::currentDateTime().addDays(1),current,wayPoint});
    m_progress.append(0);
    m_running.append(false);
    endInsertRows();

}

void AircraftModel::resetProgress(int index) {
    if (index >= 0 && index < m_aircrafts.size()) {
        m_progress[index] = 0;
        m_running[index]=true;
        AircraftData& aircraft = m_aircrafts[index];
        const WayPoint &wp0 = aircraft.path.first();
        aircraft.current = QGeoCoordinate(wp0.latitude, wp0.longitude);
        aircraft.isInsideHanoi = false;
        aircraft.rotation=0.0;
        emit dataChanged(this->index(index), this->index(index), {CoordinateRole,InsideHanoiRole,RotationRole});
    }
}

void AircraftModel::updateFlight(int index, const QVariantList &pathList){
    if (index >= 0 && index < m_aircrafts.size()){
        QVector<WayPoint> wayPoint;
        for (const QVariant &point : pathList) {
            QVariantMap map = point.toMap();
            wayPoint.append(WayPoint(map["latitude"].toDouble(),map["longitude"].toDouble()));

        }
        m_progress[index] = 0;
        m_running[index]=true;
        AircraftData& aircraft = m_aircrafts[index];
        aircraft.path=wayPoint;
        const WayPoint &wp0 = aircraft.path.first();
        aircraft.current = QGeoCoordinate(wp0.latitude, wp0.longitude);
        aircraft.isInsideHanoi = false;
        aircraft.rotation=0.0;
        emit dataChanged(this->index(index), this->index(index), {CoordinateRole,InsideHanoiRole,RotationRole});
    }
}

void AircraftModel::setPolygonModel(PolygonModel* polygonModel){
    m_polygonModel = polygonModel;
}

void AircraftModel::loadDataAsync() {
    QtConcurrent::run([this]() {
        FlightDatabaseManager dbManager;
        dbManager.connectToDatabase("localhost",5432,"aircraft","cuongceg","cuong1708");
        QVector<FlightData> loaded = dbManager.getAllFlights();
        QVector<AircraftData> res;
        res.reserve(loaded.size());

        for (const FlightData &f : loaded) {
            res.append(convertToAircraftData(f));
            m_progress.append(0.0);
            m_running.append(true);
        }

        QMetaObject::invokeMethod(this, [this, res]() {
            beginResetModel();
            m_aircrafts = res;
            endResetModel();
        }, Qt::QueuedConnection);
    });
}
AircraftData AircraftModel::convertToAircraftData(const FlightData &flight) {
    AircraftData aircraft;
    aircraft.icao24 = flight.planeId;
    aircraft.startTime = flight.startTime;
    aircraft.endTime = flight.endTime;
    aircraft.path = flight.path;

    if (!flight.path.isEmpty()) {
        const WayPoint &firstPoint = flight.path.first();
        aircraft.current = QGeoCoordinate(firstPoint.latitude, firstPoint.longitude);
    }

    aircraft.rotation = 0.0;
    aircraft.isInsideHanoi = false;

    return aircraft;
}
