cmake_minimum_required(VERSION 3.16)
project(cutefishsystem LANGUAGES CXX)

# ----------------------------
# 源文件
# ----------------------------
set(SYSTEM_SRCS
    wallpaper.cpp
    wallpaper.h
    plugin.cpp
)

# ----------------------------
# Qt6 模块
# ----------------------------
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Quick
    Gui
    DBus
)

# ----------------------------
# 创建 QML 模块
# ----------------------------
qt6_add_qml_module(cutefishsystem_qmlplugins
    URI Cutefish.System
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish/System
    SOURCES ${SYSTEM_SRCS}
    # 如果有 QML 文件，可在 QML_FILES 添加
    QML_FILES
)

# ----------------------------
# 链接库
# ----------------------------
target_link_libraries(cutefishsystem_qmlplugins PRIVATE 
    Qt6::Core
    Qt6::Quick
    Qt6::Gui
    Qt6::DBus
)

# ----------------------------
# 安装
# ----------------------------
# qt6_add_qml_module 会自动生成 qmldir 并安装到 QT6_INSTALL_QMLDIR
# 不再需要手动 install()
