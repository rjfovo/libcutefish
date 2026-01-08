cmake_minimum_required(VERSION 3.16)
project(cutefishbluez LANGUAGES CXX)

set(BLUEZQTEXTENSIONPLUGIN_SRCS
    declarativemanager.cpp
    declarativeadapter.cpp
    # declarativebattery.cpp
    declarativedevice.cpp
    declarativeinput.cpp
    declarativemediaplayer.cpp
    declarativedevicesmodel.cpp
    bluezqtextensionplugin.cpp
    applet/devicesproxymodel.cpp
    applet/bluetoothagent.cpp
    applet/bluetoothmanager.cpp
)

# ----------------------------
# Qt6 模块
# ----------------------------
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Qml
    DBus
    Quick
)

# ----------------------------
# KF6 BluezQt
# ----------------------------
find_package(KF6BluezQt REQUIRED)

# ----------------------------
# QML 文件
# ----------------------------
set(QML_FILES
    DevicesModel.qml
)

# ----------------------------
# 创建 QML 模块
# ----------------------------
qt6_add_qml_module(cutefishbluez_qmlplugins
    URI Cutefish.Bluez
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Cutefish/Bluez
    SOURCES ${BLUEZQTEXTENSIONPLUGIN_SRCS}
    QML_FILES ${QML_FILES}
)

# ----------------------------
# 链接库
# ----------------------------
target_link_libraries(cutefishbluez_qmlplugins PRIVATE 
    Qt6::Core
    Qt6::Qml
    Qt6::DBus
    KF6::BluezQt
)

# 设置rpath，确保插件库能找到安装的库
set_target_properties(cutefishbluez_qmlplugins PROPERTIES
    INSTALL_RPATH "$ORIGIN/../../.."
    BUILD_WITH_INSTALL_RPATH TRUE
    SKIP_BUILD_RPATH FALSE
)

# ----------------------------
# 安装
# ----------------------------
# qt6_add_qml_module 会自动生成 qmldir 并安装到 QT6_INSTALL_QMLDIR
# 不再需要手动 install()
