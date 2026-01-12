cmake_minimum_required(VERSION 3.16)
project(cutefishnm LANGUAGES CXX)

# ----------------------------
# 源文件
# ----------------------------
set(NETWORKMGR_SRCS
    appletproxymodel.cpp
    appletproxymodel.h

    activeconnection.cpp
    activeconnection.h

    networkitemslist.cpp
    networkitemslist.h

    networkmodel.cpp
    networkmodel.h

    networkmodelitem.cpp
    networkmodelitem.h

    wiressitemsettings.cpp
    wiressitemsettings.h

    uiutils.cpp
    uiutils.h

    identitymodel.cpp
    identitymodel.h

    handler.cpp
    handler.h

    configuration.cpp
    configuration.h

    enabledconnections.cpp
    enabledconnections.h

    enums.cpp
    enums.h

    wifisettings.cpp
    wifisettings.h

    qmlplugins.cpp
    qmlplugins.h
        qml_register.cpp
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
# KF6 模块
# ----------------------------
find_package(KF6NetworkManagerQt REQUIRED)
find_package(KF6ModemManagerQt REQUIRED)

# ----------------------------
# 创建 QML 模块
# ----------------------------
qt6_add_qml_module(cutefishnm_qmlplugins
    URI Cutefish.NetworkManagement
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Cutefish/NetworkManagement
    SOURCES ${NETWORKMGR_SRCS}
    # 如果有 QML 文件，可在 QML_FILES 添加
    QML_FILES
)

# ----------------------------
# 生成 export header
# ----------------------------
include(GenerateExportHeader)
generate_export_header(cutefishnm_qmlplugins
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/networkmanager_export.h
    BASE_NAME NetworkManager
)

# ----------------------------
# 链接库
# ----------------------------
target_link_libraries(cutefishnm_qmlplugins PRIVATE 
    Qt6::Core
    Qt6::Quick
    Qt6::Gui
    Qt6::DBus
    KF6::NetworkManagerQt
    KF6::ModemManagerQt
)

# 设置rpath，确保插件库能找到安装的库
set_target_properties(cutefishnm_qmlplugins PROPERTIES
    INSTALL_RPATH "$ORIGIN/../../.."
    BUILD_WITH_INSTALL_RPATH TRUE
    SKIP_BUILD_RPATH FALSE
)

# ----------------------------
# 安装
# ----------------------------
# qt6_add_qml_module 自动生成 qmldir 并安装到 QT6_INSTALL_QMLDIR
# 不再需要手动 install()

# 某些打包/构建环境（debian dh_auto_install）可能没有将
# qt6_add_qml_module 生成的 QML 文件正确收集到包中，
# 在这种情况下显式安装生成目录以确保 QML 模块出现在
# ${INSTALL_QMLDIR}/Cutefish/NetworkManagement
if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/Cutefish/NetworkManagement)
    if (NOT DEFINED INSTALL_QMLDIR OR INSTALL_QMLDIR STREQUAL "")
        set(INSTALL_QMLDIR "${CMAKE_INSTALL_DATAROOTDIR}/qml")
    endif()
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Cutefish/NetworkManagement
            DESTINATION ${INSTALL_QMLDIR}/Cutefish)
endif()

# Ensure the built supporting shared library is installed so the QML plugin
# can find its DT_NEEDED dependency (installed into the system library dir).
include(GNUInstallDirs)
install(TARGETS cutefishnm_qmlplugins
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
