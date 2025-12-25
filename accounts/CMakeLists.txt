cmake_minimum_required(VERSION 3.16)

project(cutefishaccounts LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(ACCOUNTS_SRCS
    accountsmanager.cpp
    accountsmanager.h
    accountsmanager_p.h
    useraccount.cpp
    useraccount.h
    useraccount_p.h
    usersmodel.cpp
    usersmodel.h
    usersmodel_p.h
    qmlplugins.cpp
)

# ----------------------------
# DBus interface (Qt6)
# ----------------------------
qt6_add_dbus_interface(
    ACCOUNTS_SRCS
    org.freedesktop.Accounts.User.xml
    user_interface
)

qt6_add_dbus_interface(
    ACCOUNTS_SRCS
    org.freedesktop.Accounts.xml
    accounts_interface
)

# ----------------------------
# Qt6 QML MODULE
# ----------------------------
qt6_add_qml_module(cutefishaccounts_qmlplugins
    URI Cutefish.Accounts
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish/Accounts
    QML_FILES
        # 如果有 QML 文件放这里，否则保持空
    SOURCES
        ${ACCOUNTS_SRCS}
)

# ----------------------------
# 依赖
# ----------------------------
target_link_libraries(cutefishaccounts_qmlplugins PRIVATE 
    Qt6::Core
    Qt6::Gui
    Qt6::DBus
    Qt6::Quick
)

# Qt6 会自动安装 QML 插件，因此不再需要旧式 install()
