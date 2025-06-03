#ifndef POLYGONLOADER_H
#define POLYGONLOADER_H
#pragma once
#include <QObject>
#include <QList>
#include <QGeoCoordinate>

class PolygonLoader : public QObject {
    Q_OBJECT
public:
    explicit PolygonLoader(QObject *parent = nullptr);
    Q_INVOKABLE QList<QGeoCoordinate> loadHanoiPolygon(const QString &filePath);
};
#endif // POLYGONLOADER_H
