#ifndef FLIGHTLOADER_H
#define FLIGHTLOADER_H
#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include "../model/AircraftModel.h"

class FlightLoader : public QObject {
    Q_OBJECT
public:
    explicit FlightLoader(QObject *parent = nullptr);
    AircraftData loadAircraftFromJson(const QString &filePath);
};
#endif // FLIGHTLOADER_H
