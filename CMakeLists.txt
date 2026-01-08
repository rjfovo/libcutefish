cmake_minimum_required(VERSION 3.16)

project(libcutefish LANGUAGES CXX)

# --------------------------------------------------
# CMake / Qt 基础设置
# --------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GNUInstallDirs)

# --------------------------------------------------
# Qt6 模块
# --------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    QuickControls2
    Widgets
    DBus
    Xml
    Concurrent
)

# --------------------------------------------------
# KDE Frameworks 6
# --------------------------------------------------
find_package(KF6CoreAddons REQUIRED)
find_package(KF6I18n REQUIRED)
find_package(KF6Config REQUIRED)
find_package(KF6Service REQUIRED)
find_package(KF6KIO REQUIRED)

# --------------------------------------------------
# 子模块（每个子模块内部必须使用 qt6_add_qml_module）
# --------------------------------------------------
add_subdirectory(accounts)
add_subdirectory(bluez)
add_subdirectory(mpris)
add_subdirectory(networkmanagement)
add_subdirectory(screen)
add_subdirectory(system)
add_subdirectory(audio)

# --------------------------------------------------
# 顶层 QML 模块：cutefish
#
# QML 中使用：
#   import cutefish 1.0
# --------------------------------------------------
qt6_add_qml_module(cutefish_qmlplugins
    URI cutefish
    VERSION 1.0
    SOURCES
        cutefishplugin.cpp
)

target_link_libraries(cutefish_qmlplugins PRIVATE
    Qt6::Core
    Qt6::Quick
)

# --------------------------------------------------
# 说明（重要）：
#
# 1. 不需要手动 install(TARGETS ...) 到 qt6/qml
# 2. 不需要手动创建 symlink
# 3. 不需要指定 OUTPUT_DIRECTORY
#
# qt6_add_qml_module 会自动：
#   - 生成 qmldir
#   - 安装到：
#       /usr/lib/x86_64-linux-gnu/qt6/qml/cutefish
#   - 注册 QML import
# --------------------------------------------------

# --------------------------------------------------
# 兼容模块安装：为旧项目提供 Cutefish.System 兼容
#
# 这是一个转发模块（shim），没有 C++ 插件
# 只包含 qmldir 文件，将 Cutefish.System 转发到 cutefish.system
# --------------------------------------------------
install(
    DIRECTORY compat/Cutefish
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/qt6/qml
)
