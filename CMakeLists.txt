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
# 顶层聚合插件
# -------------------------------
# 创建一个顶层的聚合插件，作为 cutefish 模块的入口点
qt6_add_qml_module(cutefish_qmlplugins
    URI cutefish
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish
    SOURCES cutefishplugin.cpp
)

# 链接必要的 Qt 模块
target_link_libraries(cutefish_qmlplugins PRIVATE 
    Qt6::Core
    Qt6::Quick
)

# 重命名插件文件以匹配期望的名称
# qt6_add_qml_module 默认生成 libcutefish_qmlpluginsplugin.so
# 但我们需要 libcutefishplugin.so
add_custom_command(TARGET cutefish_qmlplugins POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        $<TARGET_FILE_NAME:cutefish_qmlplugins>
        ${CMAKE_CURRENT_BINARY_DIR}/cutefish/libcutefishplugin.so
    COMMENT "Creating symlink libcutefishplugin.so -> $<TARGET_FILE_NAME:cutefish_qmlplugins>"
)

# 安装时也创建符号链接
install(CODE "
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            libcutefish_qmlpluginsplugin.so
            \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/qt6/qml/cutefish/libcutefishplugin.so
    )
")

# -------------------------------
# QML 安装路径由 qt6_add_qml_module 自动处理
# -------------------------------
message(STATUS "Use qt6_add_qml_module() in each submodule; no need for QT6_INSTALL_QMLDIR")
