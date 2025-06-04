#include "GeoUtils.h"
#include <QPolygonF>

bool isPointInPolygon(const QGeoCoordinate &point, const QList<QGeoCoordinate> &polygon) {
    QPolygonF qtPolygon;
    for (const auto &coord : polygon) {
        qtPolygon << QPointF(coord.longitude(), coord.latitude()); // x = lon, y = lat
    }

    QPointF pt(point.longitude(), point.latitude());
    return qtPolygon.containsPoint(pt, Qt::OddEvenFill);
}
