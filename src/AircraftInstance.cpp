#include "AircraftInstance.h"

AircraftInstance::AircraftInstance(QObject *parent)
    : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &AircraftInstance::updatePosition);
}

void AircraftInstance::start() {
    if (!m_timer.isActive())
        m_timer.start(1000);
}

void AircraftInstance::stop() {
    m_timer.stop();
}

bool AircraftInstance::isRunning() const {
    return m_timer.isActive();
}

void AircraftInstance::updatePosition() {
    if (data.route.isEmpty() || data.isFinished) {
        m_timer.stop();
        data.isFinished = true;
        emit finished();
        return;
    }

    data.currentCoordinate = data.route.takeFirst();
    // Cập nhật inTime / outTime / rotation nếu cần
    emit updated();
}
