# Gọi các thư mục con để build thư viện
add_subdirectory(model)
add_subdirectory(services)

# Liên kết các thư viện Qt + thư viện con
target_link_libraries(aircraft_hanoi
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::Positioning
        Qt${QT_VERSION_MAJOR}::Sql
        Qt${QT_VERSION_MAJOR}::Concurrent
        Qt${QT_VERSION_MAJOR}::Gui
        model
        services
)

# Nếu dùng Qt6 thì thêm plugin & finalize
if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(aircraft_hanoi)
    qt_finalize_executable(aircraft_hanoi)
endif()
