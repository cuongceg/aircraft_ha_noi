#include "FlightDatabaseManager.h"
#include "GeoUtils.h"
#include "AircraftManager.h"
#include <QtMath>

AircraftModel::AircraftModel(QObject *parent) : QAbstractListModel(parent)
{
    QGeoCoordinate startPoint = QGeoCoordinate(20.67, 106.05);
    QGeoCoordinate endPoint = QGeoCoordinate(21.0, 105.8);
    QList<QGeoCoordinate> routes = {startPoint};
    double rotation = qAtan2(endPoint.longitude() - startPoint.longitude(),
                             endPoint.latitude()  - startPoint.latitude()) * 180.0 / M_PI;

    QGeoCoordinate startPoint2 = QGeoCoordinate(18.76,105.63);
    QGeoCoordinate endPoint2 = QGeoCoordinate(22.09,105.52);
    QList<QGeoCoordinate> routes2 = {startPoint2};
    double rotation2 = qAtan2(endPoint2.longitude() - startPoint2.longitude(),
                             endPoint2.latitude()  - startPoint2.latitude()) * 180.0 / M_PI;

    m_aircrafts.append({"VDT001","SE1",startPoint,endPoint,QDateTime::currentDateTime(),QDateTime::currentDateTime(),startPoint,rotation,routes});
    m_aircrafts.append({"VDT002","SE2",startPoint2,endPoint2,QDateTime::currentDateTime(),QDateTime::currentDateTime(),startPoint2,rotation2,routes2});
    m_progress.append(0.0);
    m_progress.append(0.0);
    m_running.append(false);
    m_running.append(false);
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
        if (!m_running[i]) continue;

        if (m_progress[i] >= 1.0){
            if(!m_aircrafts[i].isFinished){
                m_running[i] = false;
                m_aircrafts[i].isFinished=true;
                AircraftData aircraft = m_aircrafts[i];
                FlightDatabaseManager dbManager;
                Flight flight;
                flight.startPoint= aircraft.startPoint;
                flight.endPoint = aircraft.endPoint;
                flight.planId = aircraft.planId;
                flight.route = aircraft.route;
                flight.inTime= aircraft.inTime;
                flight.outTime = aircraft.outTime;
                dbManager.insertFlight(flight);
            }
            continue;
        }
        AircraftData &aircraft = m_aircrafts[i]; // sửa từ bản sao sang tham chiếu
        m_progress[i] += 0.1;

        const QGeoCoordinate &start = aircraft.startPoint;
        const QGeoCoordinate &end = aircraft.endPoint;

        QGeoCoordinate newPos(
            start.latitude() + (end.latitude() - start.latitude()) * m_progress[i],
            start.longitude() + (end.longitude() - start.longitude()) * m_progress[i]
            );

        if (aircraft.currentCoordinate != newPos) {
            aircraft.currentCoordinate = newPos;
            aircraft.route.append(newPos);

            if (isPointInPolygon(newPos, m_hanoiPolygon) && !aircraft.isInsideHanoi) {
                aircraft.isInsideHanoi = true;
                aircraft.inTime = QDateTime::currentDateTime();
                aircraft.outTime = QDateTime::currentDateTime();
                qDebug() << "Aircraft" << aircraft.planId << "entered Hanoi";
            } else if (!isPointInPolygon(newPos, m_hanoiPolygon) && aircraft.isInsideHanoi) {
                aircraft.isInsideHanoi = false;
                aircraft.outTime = QDateTime::currentDateTime();
                qDebug() << "Aircraft" << aircraft.planId << "left Hanoi";
            }

            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, {CoordinateRole, InsideHanoiRole});
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

void AircraftModel::addAircraft(QString planeId,QGeoCoordinate startPoint,QGeoCoordinate endPoint) {
    beginInsertRows(QModelIndex(), m_aircrafts.size(), m_aircrafts.size());
    QList<QGeoCoordinate> routes = {startPoint};
    double rotation = qAtan2(endPoint.longitude() - startPoint.longitude(),
                             endPoint.latitude()  - startPoint.latitude()) * 180.0 / M_PI;
    m_aircrafts.append({"0",planeId,startPoint,endPoint,QDateTime::currentDateTime(),QDateTime::currentDateTime(),startPoint,rotation,routes});
    m_progress.append(0.0);
    m_running.append(false);
    endInsertRows();
}

void AircraftModel::resetProgress(int index) {
    if (index >= 0 && index < m_aircrafts.size()) {
        m_progress[index] = 0.0;
        m_running[index]=true;
        m_aircrafts[index].currentCoordinate = m_aircrafts[index].startPoint;
        m_aircrafts[index].route.clear();
        m_aircrafts[index].isInsideHanoi = false;
        emit dataChanged(this->index(index), this->index(index), {CoordinateRole,InsideHanoiRole});
    }
}

void AircraftModel::updateFlights(int index,QGeoCoordinate startPoint,QGeoCoordinate endPoint){
    if (index >= 0 && index < m_aircrafts.size()) {
        m_aircrafts[index].startPoint=startPoint;
        m_aircrafts[index].endPoint=endPoint;
        m_progress[index] = 0.0;
        m_running[index]=true;
        m_aircrafts[index].isFinished=false;
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
