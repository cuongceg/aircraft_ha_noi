#include "FlightModel.h"
#include "FlightDatabaseManager.h"

FlightModel::FlightModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int FlightModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_flights.count();
}

QVariant FlightModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_flights.count())
        return QVariant();

    const Flight &flight = m_flights.at(index.row());

    switch (role) {
    case FlightIdRole: return flight.id;
    case AircraftIdRole: return flight.planId;
    case InTimeRole: return flight.inTime;
    case OutTimeRole: return flight.outTime;
    case StartPointRole: return QVariant::fromValue(flight.startPoint);
    case EndPointRole: return QVariant::fromValue(flight.route.last());
    case RouteRole: {
        QVariantList list;
        for (const QGeoCoordinate &coord : flight.route) {
            QVariantMap point;
            point["latitude"] = coord.latitude();
            point["longitude"] = coord.longitude();
            list.append(point);
        }
        return list;
    }
    default: return QVariant();
    }
}

QHash<int, QByteArray> FlightModel::roleNames() const {
    return {
        { FlightIdRole, "flightId" },
        { AircraftIdRole, "aircraftId" },
        { InTimeRole, "inTime" },
        { OutTimeRole, "outTime" },
        { RouteRole, "route" },
        {StartPointRole,"startPoint"},
        {EndPointRole,"endPoint"}
    };
}

void FlightModel::loadFlights() {
    beginResetModel();
    m_flights.clear();
    FlightDatabaseManager dbManager;
    m_flights = dbManager.loadAllFlights();
    endResetModel();
}


void FlightModel::searchFlights(QString planeId,QDate date){
    beginResetModel();
    m_flights.clear();
    qDebug()<<planeId<<" "<<date;
    FlightDatabaseManager dbManager;
    m_flights = dbManager.searchFlights(planeId,date);
    qDebug()<<"Size"<<m_flights.size();
    endResetModel();
}
