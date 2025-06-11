#ifndef POLYGONMODEL_H
#define POLYGONMODEL_H
#pragma once

#include <QAbstractListModel>
#include <QGeoCoordinate>
#include <QVector>

struct PolygonData {
    QList<QGeoCoordinate> path;
};

class PolygonModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum PolygonRoles {
        PathRole = Qt::UserRole + 1
    };

    explicit PolygonModel(QObject* parent = nullptr);

    // Basic overrides
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Q_INVOKABLE method to add a polygon from QML
    Q_INVOKABLE void addPolygon(const QVariantList& coordinateList);
    Q_INVOKABLE void updatePolygon(const QVariantList& coordinateList,int index);
    bool isInsidePolygons(const QGeoCoordinate &point);

private:
    QList<PolygonData> m_polygons;
};

#endif // POLYGONMODEL_H
