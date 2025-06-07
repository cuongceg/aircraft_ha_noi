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
    default: return QVariant();
    }
}

QHash<int, QByteArray> FlightModel::roleNames() const {
    return {
        { FlightIdRole, "flightId" },
        { AircraftIdRole, "aircraftId" },
        { InTimeRole, "inTime" },
        { OutTimeRole, "outTime" }
    };
}

void FlightModel::loadFlights() {
    beginResetModel();
    m_flights.clear();
    FlightDatabaseManager dbManager;
    m_flights = dbManager.loadAllFlights();
    endResetModel();
}
