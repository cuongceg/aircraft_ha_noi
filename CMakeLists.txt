cmake_minimum_required(VERSION 3.14)

project(aircraft_hanoi VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tìm Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Positioning Sql Concurrent Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Positioning Sql Concurrent Gui)

# Khai báo file nguồn
set(SOURCES
    src/main.cpp
    qml.qrc
)

# Tạo executable
add_executable(aircraft_hanoi
    ${SOURCES}
)
# Gọi cmake con trong src/
add_subdirectory(src)
