add_library(services STATIC
    FlightDatabaseManager.cpp
    FlightDatabaseManager.h
    FlightLoader.cpp
    FlightLoader.h
    PolygonLoader.cpp
    PolygonLoader.h
    # Các file khác nếu có
)

target_include_directories(services
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../model
)

target_link_libraries(services
    PUBLIC Qt${QT_VERSION_MAJOR}::Core
    PUBLIC Qt${QT_VERSION_MAJOR}::Sql
    PUBLIC Qt${QT_VERSION_MAJOR}::Positioning
)
