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
# 子模块（每个子模块内部必须使用 qt_add_qml_module）
# --------------------------------------------------
add_subdirectory(accounts)
add_subdirectory(bluez)
add_subdirectory(mpris)
add_subdirectory(networkmanagement)
add_subdirectory(screen)
add_subdirectory(system)
add_subdirectory(audio)
add_subdirectory(cutefish)