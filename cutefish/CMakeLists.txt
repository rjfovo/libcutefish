cmake_minimum_required(VERSION 3.16)
project(cutefish LANGUAGES CXX)

# ----------------------------
# 创建 QML 模块
# ----------------------------
# 或者创建一个简单的虚拟插件作为父模块
# qt_add_qml_module(cutefish
#     URI "cutefish"
#     VERSION 1.0
#     OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish
#     # 可以不包含任何 QML_FILES 或 SOURCES
#     # 或者包含一些通用的类型
#     QML_FILES
# )

# # ----------------------------
# # 安装虚拟插件
# message(STATUS "Cutefish build dir: ${CMAKE_CURRENT_BINARY_DIR}/cutefish")
# message(STATUS "Checking if exists: ${CMAKE_CURRENT_BINARY_DIR}/cutefish")
# if (NOT DEFINED INSTALL_QMLDIR OR INSTALL_QMLDIR STREQUAL "")
#     set(INSTALL_QMLDIR "${CMAKE_INSTALL_DATAROOTDIR}/qml")
# endif()
# install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish
#         DESTINATION ${INSTALL_QMLDIR}/)

# 1. 确保目录存在
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish)

# 正确的版本声明方式
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/cutefish/qmldir
    "module cutefish\n"
    "version 1.0\n"  # 版本号单独一行
    "typeinfo plugins.qmltypes\n"
)

# 3. 创建空的 qmltypes 文件（避免Qt Creator警告）
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/cutefish/plugins.qmltypes
    "Module {\n"
    "    uri: \"cutefish\"\n"
    "    version: 1.0\n"
    "}\n"
)

# --------------------------------------------------
# 安装配置
# --------------------------------------------------
if (NOT DEFINED INSTALL_QMLDIR OR INSTALL_QMLDIR STREQUAL "")
    set(INSTALL_QMLDIR "${CMAKE_INSTALL_DATAROOTDIR}/qml")
endif()

# 只安装 qmldir 和 qmltypes，不安装任何 .so 文件
install(FILES 
    ${CMAKE_CURRENT_BINARY_DIR}/cutefish/qmldir
    ${CMAKE_CURRENT_BINARY_DIR}/cutefish/plugins.qmltypes
    DESTINATION ${INSTALL_QMLDIR}/cutefish
)
