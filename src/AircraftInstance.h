#ifndef AIRCRAFTINSTANCE_H
#define AIRCRAFTINSTANCE_H
#pragma once
#include <QGeoCoordinate>
#include<QDateTime>
#include <QTimer>




class AircraftInstance : public QObject {
    Q_OBJECT
public:
    explicit AircraftInstance(QObject *parent = nullptr);

    AircraftData data;
    void start();
    void stop();
    bool isRunning() const;

signals:
    void updated();      // phát khi dữ liệu thay đổi
    void finished();     // phát khi kết thúc mô phỏng

private slots:
    void updatePosition();

private:
    QTimer m_timer;
};

#endif // AIRCRAFTINSTANCE_H
