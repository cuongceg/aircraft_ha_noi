add_library(model STATIC
    FlightModel.cpp
    FlightModel.h
    AircraftModel.cpp
    AircraftModel.h
    Flight.h
    PolygonModel.cpp
    PolygonModel.h
)

target_include_directories(model
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../services
)

target_link_libraries(model
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Sql
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Positioning
        Qt${QT_VERSION_MAJOR}::Concurrent   # ✅ Không để riêng dòng
)
