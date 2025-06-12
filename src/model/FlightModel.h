// #ifndef FLIGHTMODEL_H
// #define FLIGHTMODEL_H
// #include <QAbstractListModel>
// #include <QDateTime>
// #include <QDebug>
// #include "Flight.h"
// class FlightModel : public QAbstractListModel
// {
//     Q_OBJECT
// public:
//     enum FlightRoles {
//         FlightIdRole = Qt::UserRole + 1,
//         AircraftIdRole,
//         InTimeRole,
//         OutTimeRole,
//         RouteRole,
//         StartPointRole,
//         EndPointRole
//     };

//     explicit FlightModel(QObject *parent = nullptr);

//     int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
//     QHash<int, QByteArray> roleNames() const override;

//     Q_INVOKABLE void loadFlights();
//     Q_INVOKABLE void searchFlights(QString planeId,QDate date);

// private:
//     QList<Flight> m_flights;
// };
// #endif // FLIGHTMODEL_H
