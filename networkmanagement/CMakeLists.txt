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

# ----------------------------
# 安装
# ----------------------------
# qt6_add_qml_module 自动生成 qmldir 并安装到 QT6_INSTALL_QMLDIR
# 不再需要手动 insta
