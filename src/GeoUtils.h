#ifndef GEOUTILS_H
#define GEOUTILS_H
#pragma once
#include <QList>
#include <QGeoCoordinate>

bool isPointInPolygon(const QGeoCoordinate &point, const QList<QGeoCoordinate> &polygon);
#endif // GEOUTILS_H
