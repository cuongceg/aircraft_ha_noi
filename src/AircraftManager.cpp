#include "FlightDatabaseManager.h"
#include "GeoUtils.h"
#include <QtMath>

AircraftModel::AircraftModel(QObject *parent) : QAbstractListModel(parent)
{
    QGeoCoordinate startPoint = QGeoCoordinate(20.67, 106.05);
    QGeoCoordinate endPoint = QGeoCoordinate(21.0, 105.8);
    QList<QGeoCoordinate> routes = {startPoint};
    double rotation = qAtan2(endPoint.longitude() - startPoint.longitude(),
                             endPoint.latitude()  - startPoint.latitude()) * 180.0 / M_PI;
    m_aircrafts.append({"VDT001","SE1",startPoint,endPoint,QDateTime::currentDateTime(),QDateTime::currentDateTime(),startPoint,rotation,routes});
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
        return QVariant::fromValue(aircraft.currentCoordinate);
    if (role == InsideHanoiRole)
        return QVariant(aircraft.isInsideHanoi);
    if (role == RotationRole)
        return QVariant(aircraft.rotation);
    if(role == StartPointRole)
        return QVariant::fromValue(aircraft.startPoint);
    if(role == EndPointRole)
        return QVariant::fromValue(aircraft.endPoint);
    return QVariant();
}

QHash<int, QByteArray> AircraftModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CoordinateRole] = "coordinate";
    roles[InsideHanoiRole] = "insideHanoi";
    roles[RotationRole] = "rotation";
    roles[StartPointRole] = "startPoint";
    roles[EndPointRole] = "endPoint";
    return roles;
}

void AircraftModel::updatePositions()
{
    for (int i = 0; i < m_aircrafts.size(); ++i) {
        AircraftData aircraft = m_aircrafts[i];
        m_progress[i] += 0.1;
        if (m_progress[i] > 1.0){
            // FlightDatabaseManager dbManager;
            // dbManager.insertFlight(aircraft);
            continue;
        }

        const QGeoCoordinate &start = aircraft.startPoint;
        const QGeoCoordinate &end = aircraft.endPoint;

        QGeoCoordinate newPos(
            start.latitude() + (end.latitude() - start.latitude()) * m_progress[i],
            start.longitude() + (end.longitude() - start.longitude()) * m_progress[i]
        );

        if (aircraft.currentCoordinate != newPos) {
            m_aircrafts[i].currentCoordinate = newPos;
            m_aircrafts[i].route.append(newPos);
            if(isPointInPolygon(newPos,m_hanoiPolygon) && !aircraft.isInsideHanoi){
                m_aircrafts[i].isInsideHanoi = true;
                m_aircrafts[i].inTime = QDateTime::currentDateTime();
                m_aircrafts[i].outTime = QDateTime::currentDateTime();
                qDebug() << "Aircraft is inside Hanoi";
            }else if(!isPointInPolygon(newPos,m_hanoiPolygon) && aircraft.isInsideHanoi){
                m_aircrafts[i].isInsideHanoi = false;
                m_aircrafts[i].outTime = QDateTime::currentDateTime();
                qDebug() << "Outside Hanoi";
            }
            // emit change
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, {CoordinateRole,InsideHanoiRole,RotationRole});
        }
    }
}

void AircraftModel::resetProgress(int index) {
    if (index >= 0 && index < m_aircrafts.size()) {
        m_progress[index] = 0.0;
        m_aircrafts[index].currentCoordinate = m_aircrafts[index].startPoint;
        m_aircrafts[index].route.clear();
        m_aircrafts[index].isInsideHanoi = false;
        emit dataChanged(this->index(index), this->index(index), {CoordinateRole,InsideHanoiRole});
    }
}

void AircraftModel::updateFlights(int index,QGeoCoordinate startPoint,QGeoCoordinate endPoint){
    qDebug() << "I'm not here" << index << " " << startPoint <<" " << endPoint;
    if (index >= 0 && index < m_aircrafts.size()) {
        qDebug() << "Update Flight with" <<startPoint<<" "<<endPoint;
        m_aircrafts[index].startPoint=startPoint;
        m_aircrafts[index].endPoint=endPoint;
        m_progress[index] = 0.0;
        m_aircrafts[index].currentCoordinate = startPoint;
        m_aircrafts[index].route.clear();
        m_aircrafts[index].isInsideHanoi = false;
        m_aircrafts[index].rotation=qAtan2(endPoint.longitude() - startPoint.longitude(),
                                             endPoint.latitude()  - startPoint.latitude()) * 180.0 / M_PI;
        emit dataChanged(this->index(index), this->index(index), {CoordinateRole,InsideHanoiRole,StartPointRole,EndPointRole,RotationRole});
    }
}

void AircraftModel::setPolygon(QList<QGeoCoordinate>& hanoiPolygon){
    m_hanoiPolygon = hanoiPolygon;
}

void AircraftModel::getStartAndEndPoint(){
    for(int i = 0; i < m_aircrafts.size(); ++i){
        QModelIndex idx = index(i);
        emit dataChanged(idx, idx, {StartPointRole,EndPointRole});
    }
}
