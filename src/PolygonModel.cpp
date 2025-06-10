#include "PolygonModel.h"
#include <QPolygonF>

PolygonModel::PolygonModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int PolygonModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_polygons.count();
}

QVariant PolygonModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_polygons.size())
        return QVariant();

    const PolygonData& polygon = m_polygons[index.row()];

    if (role == PathRole) {
        QVariantList path;
        for (const auto& coord : polygon.path) {
            QVariantMap point;
            point["latitude"] = coord.latitude();
            point["longitude"] = coord.longitude();
            path << point;
        }
        return path;
    }

    return QVariant();
}

QHash<int, QByteArray> PolygonModel::roleNames() const {
    return {
        { PathRole, "path" }
    };
}

void PolygonModel::addPolygon(const QVariantList& coordinateList) {
    PolygonData polygon;
    for (const QVariant& coordVar : coordinateList) {
        QVariantMap map = coordVar.toMap();
        double lat = map["latitude"].toDouble();
        double lon = map["longitude"].toDouble();
        polygon.path.append(QGeoCoordinate(lat, lon));
    }

    beginInsertRows(QModelIndex(), m_polygons.size(), m_polygons.size());
    m_polygons.append(polygon);
    endInsertRows();
}

bool PolygonModel::isInsidePolygons(const QGeoCoordinate &point){
    for(PolygonData polygon : m_polygons){
        QPolygonF qtPolygon;
        for (const auto &coord : polygon.path) {
            qtPolygon << QPointF(coord.longitude(), coord.latitude());
        }

        QPointF pt(point.longitude(), point.latitude());
        if(qtPolygon.containsPoint(pt, Qt::OddEvenFill))return true;
    }
    return false;
}
