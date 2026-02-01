cmake_minimum_required(VERSION 3.16)
project(cutefishscreen LANGUAGES CXX)

# ----------------------------
# 源文件
# ----------------------------
set(SCREEN_SRCS
    common/control.cpp
    common/globals.cpp
    common/orientation_sensor.cpp
    common/utils.cpp
    confighandler.cpp
    outputmodel.cpp
    screen.cpp
)

# ----------------------------
# Qt6 模块
# ----------------------------
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Quick
    Gui
    DBus
    Sensors
    Test
)

# ----------------------------
# KF6 模块
# ----------------------------
find_package(KF6Screen REQUIRED)
find_package(KF6KIO REQUIRED)

# ----------------------------
# 创建 QML 模块
# ----------------------------
qt_add_qml_module(cutefishscreen_qmlplugins
    URI "cutefish.screen"
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish/screen
    SOURCES ${SCREEN_SRCS}
    # 如果有 QML 文件，可在 QML_FILES 添加
    QML_FILES
)

# ----------------------------
# 链接库
# ----------------------------
target_link_libraries(cutefishscreen_qmlplugins PRIVATE 
    Qt6::Core
    Qt6::Quick
    Qt6::Gui
    Qt6::DBus
    Qt6::Sensors
    Qt6::Test
    KF6::Screen
    KF6::KIOCore
)

# 设置rpath，确保插件库能找到安装的库
set_target_properties(cutefishscreen_qmlplugins PROPERTIES
    INSTALL_RPATH "$ORIGIN/../../.."
    BUILD_WITH_INSTALL_RPATH TRUE
    SKIP_BUILD_RPATH FALSE
)

# ----------------------------
# 安装
# ----------------------------
if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/cutefish/screen)
    if (NOT DEFINED INSTALL_QMLDIR OR INSTALL_QMLDIR STREQUAL "")
        set(INSTALL_QMLDIR "${CMAKE_INSTALL_DATAROOTDIR}/qml")
    endif()
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish/screen
            DESTINATION ${INSTALL_QMLDIR}/cutefish)
endif()

# 安装主库文件到系统库目录
install(TARGETS cutefishscreen_qmlplugins
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
