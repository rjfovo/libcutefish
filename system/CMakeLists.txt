cmake_minimum_required(VERSION 3.16)
project(cutefishsystem LANGUAGES CXX)

# ----------------------------
# 源文件
# ----------------------------
set(SYSTEM_SRCS
    wallpaper.cpp
    wallpaper.h
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
qt_add_qml_module(cutefishsystem_qmlplugins
    URI "cutefish.system"
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish/system
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

# 设置rpath，确保插件库能找到安装的库
set_target_properties(cutefishsystem_qmlplugins PROPERTIES
    INSTALL_RPATH "$ORIGIN/../../.."
    BUILD_WITH_INSTALL_RPATH TRUE
    SKIP_BUILD_RPATH FALSE
)

# ----------------------------
# 安装
# ----------------------------
# qt6_add_qml_module 会自动生成 qmldir 并安装到 QT6_INSTALL_QMLDIR
# 不再需要手动 install()

# 某些打包/构建环境可能没有将 qt6_add_qml_module 生成的 QML 文件正确收集到包中，
# 在这种情况下显式安装生成目录以确保 QML 模块出现在 ${INSTALL_QMLDIR}/cutefish
if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/cutefish/system)
    if (NOT DEFINED INSTALL_QMLDIR OR INSTALL_QMLDIR STREQUAL "")
        set(INSTALL_QMLDIR "${CMAKE_INSTALL_DATAROOTDIR}/qml")
    endif()
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish/system
            DESTINATION ${INSTALL_QMLDIR}/cutefish)
endif()

# 安装主库文件到系统库目录
install(TARGETS cutefishsystem_qmlplugins
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
