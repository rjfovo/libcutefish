cmake_minimum_required(VERSION 3.16)
project(cutefishmpris LANGUAGES CXX)

# ----------------------------
# 源文件
# ----------------------------
set(NETWORKMGR_SRCS
    mpris.cpp
    mpris.h
    mpriscontroller_p.h
    mpriscontroller.cpp
    mpriscontroller.h
    mprismanager.cpp
    mprismanager.h
    mprisplayer.cpp
    mprisplayer.h
    mprisplayeradaptor.cpp
    mprisplayerinterface.cpp
    mprisqt.h
    mprisrootadaptor.cpp
    mprisrootinterface.cpp
    qmlplugins.cpp

    qtdbusextended/dbusextended.h
    qtdbusextended/dbusextendedabstractinterface.cpp
    qtdbusextended/dbusextendedabstractinterface.h
    qtdbusextended/dbusextendedpendingcallwatcher_p.h
    qtdbusextended/dbusextendedpendingcallwatcher.cpp
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
# 如果有 QML 文件可以列在这里
# ----------------------------
set(QML_FILES
    # qml 文件可添加在这里，例如：
    # Player.qml
)

# ----------------------------
# 创建 Qt6 QML 模块
# ----------------------------
qt6_add_qml_module(cutefishmpris_qmlplugins
    URI Cutefish.Mpris
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish/Mpris
    SOURCES ${NETWORKMGR_SRCS}
    QML_FILES ${QML_FILES}
)

# ----------------------------
# 链接库
# ----------------------------
target_link_libraries(cutefishmpris_qmlplugins PRIVATE 
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
