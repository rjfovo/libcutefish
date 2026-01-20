cmake_minimum_required(VERSION 3.16)
project(cutefish LANGUAGES CXX)

# ----------------------------
# 创建 QML 模块
# ----------------------------
# 或者创建一个简单的虚拟插件作为父模块
qt_add_qml_module(cutefish
    URI "cutefish"
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish
    # 可以不包含任何 QML_FILES 或 SOURCES
    # 或者包含一些通用的类型
    QML_FILES
)

# ----------------------------
# 安装虚拟插件
message(STATUS "Cutefish build dir: ${CMAKE_CURRENT_BINARY_DIR}/cutefish")
message(STATUS "Checking if exists: ${CMAKE_CURRENT_BINARY_DIR}/cutefish")
if (NOT DEFINED INSTALL_QMLDIR OR INSTALL_QMLDIR STREQUAL "")
    set(INSTALL_QMLDIR "${CMAKE_INSTALL_DATAROOTDIR}/qml")
endif()
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish
        DESTINATION ${INSTALL_QMLDIR}/)
