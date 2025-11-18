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
    plugin.cpp
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
qt6_add_qml_module(cutefishscreen_qmlplugins
    URI Cutefish.Screen
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Cutefish/Screen
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

# ----------------------------
# 安装
# ----------------------------
# qt6_add_qml_module 会自动生成 qmldir 并安装到 QT6_INSTALL_QMLDIR
# 不再需要手动 install()
