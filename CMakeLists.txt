cmake_minimum_required(VERSION 3.16)

project(libcutefish LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

# -------------------------------
# Qt6 模块
# -------------------------------
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

# -------------------------------
# KDE Frameworks 6 模块
# -------------------------------
find_package(KF6CoreAddons REQUIRED)
find_package(KF6I18n REQUIRED)
find_package(KF6Config REQUIRED)
find_package(KF6Service REQUIRED)
find_package(KF6KIO REQUIRED)

# -------------------------------
# 子模块
# -------------------------------
# 使用 qt6_add_qml_module 管理 QML 插件的安装
# 各子模块内部的 CMakeLists.txt 应修改为使用 qt6_add_qml_module
add_subdirectory(accounts)
add_subdirectory(bluez)
add_subdirectory(mpris)
add_subdirectory(networkmanagement)
add_subdirectory(screen)
add_subdirectory(system)
add_subdirectory(audio)

# -------------------------------
# QML 安装路径由 qt6_add_qml_module 自动处理
# -------------------------------
message(STATUS "Use qt6_add_qml_module() in each submodule; no need for QT6_INSTALL_QMLDIR")
